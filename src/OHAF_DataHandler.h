#pragma once

namespace MaxsuOnHitAnimFW
{
	using string = std::string;

	struct OHAF_AnimGraphObj
	{
		using hitFlag = RE::TESHitEvent::Flag;

		enum Type : std::uint32_t
		{
			kAny = 0,
			kAll,
			kNone,
			kTotal
		};

		const string GraphName;

		const string EventName;

		const string VarFloatName;

		const float	 MinDamage;
	
		const hitFlag flags[Type::kTotal];


		OHAF_AnimGraphObj() = delete;

		OHAF_AnimGraphObj(const OHAF_AnimGraphObj&) = delete;

		OHAF_AnimGraphObj& operator=(const OHAF_AnimGraphObj&) = delete;

		OHAF_AnimGraphObj(const string thisGraphName, const string thisEventName, const string thisVarFloatName,
			const hitFlag tAny = hitFlag::kNone, const hitFlag tAll = hitFlag::kNone, const hitFlag tNone = hitFlag::kHitBlocked, const float thisMinDamage = 1.0f
		):
			GraphName(thisGraphName), EventName(thisEventName), VarFloatName(thisVarFloatName), flags{ tAny ,tAll, tNone}, MinDamage(thisMinDamage)
		{
			logger::info(FMT_STRING("Construct a Animation Graph Object, Graph Name {}, Event Name : {}, VariableFloat Name : {}, Minimum Damage is : {}"), GraphName, EventName, VarFloatName, thisMinDamage);
			logger::info(FMT_STRING("Flag Any:{}, All:{}, None:{}"), flags[0], flags[1], flags[2]);
		}

	};


	class DataHandler
	{
	public:

		static DataHandler* GetSingleton()
		{
			static DataHandler singleton;
			return &singleton;
		}

		inline bool IsDebugLogEnable() const{ return EnableDebugLog; }

		inline bool IsModFunctionsEnable() const{return EnableModFunctions; }

		inline bool IsSpellEnable() const { return EnableSpell;}

		inline bool IsEnablePC() const { return EnablePC; }

		inline bool IsEnableNPC() const { return EnableNPC; }

	private: 

		DataHandler();

		~DataHandler() = default;

		DataHandler(const DataHandler&) = delete;

		DataHandler(DataHandler&&) = delete;

		DataHandler& operator= (const DataHandler&) = delete;

		DataHandler& operator= (DataHandler&&) = delete;


		bool EnableDebugLog = false;

		bool EnableModFunctions = true;

		bool EnableSpell = true;

		bool EnablePC = true;

		bool EnableNPC = true;
	
	//--------------------------------AnimGraph Handler---------------------------------------------------------

	public:

		std::shared_ptr<OHAF_AnimGraphObj> LookUpAGObjByName(const string graph_name);

	private:

		void AddAnimGraphObj(string GraphName, const string EventName, const string VarFloatName);

		void GetAnimGraphsFromINIFiles();

		std::unordered_map<string, std::shared_ptr<OHAF_AnimGraphObj>> AG_Map;

	//-------------------------------------------------------------------------------------------------------------


	};

}