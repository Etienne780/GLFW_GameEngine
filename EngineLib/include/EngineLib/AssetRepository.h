#pragma once
#include "EngineTypes.h"

namespace EngineCore {
	
	namespace ASSETS {

		namespace ENGINE {
			namespace TEXTURE {
				Asset_Texture2DID Missing();
				Asset_Texture2DID Cursedmod3();
			}

			namespace MESH {
				Asset_MeshID Cube();
			}

			namespace SHADER {
				Asset_ShaderID Default();
				Asset_ShaderID DefaultText();
			}

			namespace MATERIAL {
				Asset_MaterialID Default();
			}
		}
	
	}

	void LoadBaseAsset();

}