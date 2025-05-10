#pragma once
#include <string>
class ObjectTypes
{
	public:
	enum class ObjectType
	{
		BLOB,
		TREE,
		COMMIT,
		REF,
		HEAD
	};

	static std::string objectTypeToString(ObjectType type)
	{
		switch (type)
		{
			case ObjectType::BLOB: return "blob";
			case ObjectType::TREE: return "tree";
			case ObjectType::COMMIT: return "commit";
			case ObjectType::REF: return "ref";
			case ObjectType::HEAD: return "head";
			default: return "unknown";
		}
	}
};