#include "TextureHolder.h"
#include <assert.h> 


TextureHolder* TextureHolder::m_s_Instance = nullptr;

TextureHolder::TextureHolder()
{

	assert(m_s_Instance == nullptr);

	m_s_Instance = this;
}


Texture& TextureHolder::GetTexture(string const& filename){
	auto& m = m_s_Instance->m_Texture;

	auto KeyValuePair = m.find(filename);

	if(KeyValuePair != m.end())
	{
		return KeyValuePair->second;
	}else
	{
		auto& texture = m[filename];
		texture.loadFromFile(filename);

		return texture;
	}

}
