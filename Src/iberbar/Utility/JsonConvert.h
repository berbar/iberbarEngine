#pragma once

#include <nlohmann/json.hpp>
#include <optional>

namespace iberbar
{
	inline std::optional<std::string> JsonGetString( const nlohmann::json& json, const std::string& key )
	{
		if (json.contains( key ) == false)
			return std::optional<std::string>();
		auto nodeTemp = json.at( key );
		if (nodeTemp.is_string() == false)
			return std::optional<std::string>();
		return nodeTemp.get<std::string>();
	}

	inline std::string JsonGetString( const nlohmann::json& json, const std::string& key, const std::string& def )
	{
		if (json.contains( key ) == false)
			return def;
		auto nodeTemp = json.at( key );
		if (nodeTemp.is_string() == false)
			return def;
		return nodeTemp.get<std::string>();
	}

	inline int JsonGetInt32( const nlohmann::json & json, const std::string& key, int def )
	{
		if (json.contains( key ) == false)
			return def;
		auto nodeTemp = json.at( key );
		if (nodeTemp.is_number_integer() == false)
			return def;
		return nodeTemp.get<int>();
	}

	inline std::optional<int> JsonGetInt32( const nlohmann::json& json, size_t index )
	{
		auto nodeTemp = json.at( index );
		if ( nodeTemp.is_number_integer() == false )
			return std::optional<int>();
		return nodeTemp.get<int>();
	}


	inline float JsonGetFloat( const nlohmann::json& json, const std::string& key, float def )
	{
		if ( json.contains( key ) == false )
			return def;
		auto nodeTemp = json.at( key );
		if ( nodeTemp.is_number_integer() == false && nodeTemp.is_number_float() == false )
			return def;
		return nodeTemp.get<float>();
	}


	inline std::optional<float> JsonGetFloat( const nlohmann::json& json, size_t index )
	{
		auto nodeTemp = json.at( index );
		if ( nodeTemp.is_number_integer() == false )
			return std::optional<float>();
		return nodeTemp.get<float>();
	}


	inline bool JsonGetBoolean( const nlohmann::json& json, const std::string& key, bool def )
	{
		if ( json.contains( key ) == false )
			return def;
		auto nodeTemp = json.at( key );
		if ( nodeTemp.is_boolean() == false )
			return def;
		return nodeTemp.get<bool>();
	}


	inline std::optional<bool> JsonGetBoolean( const nlohmann::json& json, size_t index )
	{
		auto nodeTemp = json.at( index );
		if ( nodeTemp.is_boolean() == false )
			return std::optional<bool>();
		return nodeTemp.get<bool>();
	}


	inline int JsonGetInt32( const nlohmann::json& json, size_t index, int def )
	{
		auto nodeTemp = json.at( index );
		if ( nodeTemp.is_number_integer() == false )
			return def;
		return nodeTemp.get<int>();
	}
}
