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
			}

			namespace MATERIAL {
				MaterialID Default();
				MaterialID DefaultText();
				MaterialID DefaultUI();
			}

			namespace FONT {
				FontID Default();
			}
		}
	
	}

	void LoadBaseAsset();

}