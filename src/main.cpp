#include "version.h"
#include "MaxsuLogsBackUp.h"
#include "OHAF_LoadGameFunc.h"


extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
#ifndef NDEBUG
	auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
	auto path = logger::log_directory();
	if (!path) {
		return false;
	}

	*path /= "MaxsuOnHitAnimFW.log"sv;
	MaxsuLogsBackUpNamespace::CreateBackUpLogs(*path, 3);
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

#ifndef NDEBUG
	log->set_level(spdlog::level::trace);
#else
	log->set_level(spdlog::level::debug);
	log->flush_on(spdlog::level::debug);
#endif

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("%g(%#): [%^%l%$] %v"s);

	logger::info(FMT_STRING("MaxsuOnHitAnimFW v{}"), MYFP_VERSION_VERSTRING);

	a_info->infoVersion = SKSE::PluginInfo::kVersion;
	a_info->name = "MaxsuOnHitAnimFW";
	a_info->version = MYFP_VERSION_MAJOR;

	if (a_skse->IsEditor()) {
		logger::critical("Loaded in editor, marking as incompatible"sv);
		return false;
	}

	const auto ver = a_skse->RuntimeVersion();
	if (ver < SKSE::RUNTIME_1_5_39) {
		logger::critical(FMT_STRING("Unsupported runtime version {}"), ver.string());
		return false;
	}

	return true;
}


extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	logger::info("MaxsuOnHitAnimFW loaded");

	SKSE::Init(a_skse);

	auto g_message = SKSE::GetMessagingInterface();

	if (!g_message)
	{
		logger::error("Messaging Interface Not Found!");
		return false;
	}

	if (g_message->RegisterListener(MaxsuOnHitAnimFW::EventCallback))
	{
		logger::info("Register Event Call Back!");
		return true;
	}

	return false;
}
