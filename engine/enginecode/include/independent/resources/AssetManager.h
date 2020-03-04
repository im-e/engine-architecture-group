#pragma once

/*! \file AssetManager.h
\brief Supposed to store and manage various assets
*/

#include <string>
#include <memory>
#include <map>
#include "systems/Log.h"

namespace Engine
{
	/*! \class AssetManager
	\brief Templated class to manage various assets
	*/
	template<typename T>
	class AssetManager
	{
	private:
		std::map<std::string, std::shared_ptr<T>> m_assets; //!< Storage for the assets
	public:
		//! Default constructor
		AssetManager<T>() {};
		bool contains(const std::string& key); //!< Has the asset been already loaded? \param key key associated with an asset
		void add(const std::string& key, std::shared_ptr<T>& asset); //!< Add asset to the storage \param key key associated with an asset \param asset asset to be added
		std::shared_ptr<T> getAsset(const std::string& key); //!< Retrieve asset from storage \param key key associated with an asset
		std::map<std::string, std::shared_ptr<T>> getCollection(); //!< Get full collection of the assets \return full assets collection of specified type
		void remove(const std::string& key); //!< Removes an item from the asset manager \param key key associated with an item
	};

	template<typename T>
	inline bool AssetManager<T>::contains(const std::string & key)
	{
		//does it exist?
		if (m_assets.find(key) != m_assets.end())
			return true;

		return false;
	}

	template<typename T>
	inline void AssetManager<T>::add(const std::string & key, std::shared_ptr<T>& asset)
	{
		if (contains(key) == false)
			m_assets[key] = asset;
		else
			LogWarn("Asset at key {0} already exists.", key);
	}

	template<typename T>
	inline std::shared_ptr<T> AssetManager<T>::getAsset(const std::string & key)
	{
		if (contains(key) == true)
		{
			return m_assets[key];
		}
		else
		{
			LogError("Asset at key {0} does not exist", key);
			return nullptr;
		}
	}

	template<typename T>
	inline std::map<std::string, std::shared_ptr<T>> AssetManager<T>::getCollection()
	{
		return m_assets;
	}

	template<typename T>
	inline void AssetManager<T>::remove(const std::string & key)
	{
		if(contains(key) == true)
			m_assets.erase(key);
	}
}