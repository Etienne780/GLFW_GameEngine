#pragma once

namespace EngineCore {
	
	namespace ASSETS {

		namespace ENGINE {
			namespace TEXTURE {
				unsigned int Missing();
				unsigned int Cursedmod3();
			}

			namespace MESH {
				unsigned int Cube();
			}

			namespace SHADER {
				unsigned int Default();
			}

			namespace MATERIAL {
				unsigned int Default();
			}
		}
	
	}

	void LoadBaseAsset();

}