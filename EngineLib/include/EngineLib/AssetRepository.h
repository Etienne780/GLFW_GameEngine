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
			}

			namespace SHADER {
				ShaderID Default();
				ShaderID DefaultText();
			}

			namespace MATERIAL {
				MaterialID Default();
				MaterialID DefaultText();
			}

			namespace FONT {
				FontID Default();
			}
		}
	
	}

	void LoadBaseAsset();

}