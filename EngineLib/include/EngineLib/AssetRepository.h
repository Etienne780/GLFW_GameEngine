#pragma once
#include "EngineTypes.h"

namespace EngineCore {
	
	namespace ASSETS {

		namespace ENGINE {
			namespace TEXTURE {
				Texture2DID Missing();
				Texture2DID Cursedmod3();
			}

			namespace MESH {
				MeshID Cube();
				MeshID Plain();
				MeshID UIPlain();
			}

			namespace SHADER {
				ShaderID Default();
				ShaderID DefaultText();
				ShaderID DefaultUI();
				ShaderID DefaultDebugUI();
			}

			namespace MATERIAL {
				MaterialID Default();
				MaterialID DefaultText();
				MaterialID DefaultUI();
				MaterialID DefaultDebugUI();
			}

			namespace FONT {
				FontID Default();
			}
		}
	
	}

	void LoadBaseAsset();

}