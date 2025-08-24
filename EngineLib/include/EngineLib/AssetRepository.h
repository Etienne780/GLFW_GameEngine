#pragma once

namespace EngineCore {
	
	namespace ID{
		namespace TEXTURE {
			namespace ENGINE {
				unsigned int Missing();
			}
		}
		
		namespace MESH {
			namespace ENGINE {
				unsigned int Cube();
			}
		}

		namespace SHADER {
			namespace ENGINE {
				unsigned int Default();
				unsigned int Outline();
			}
		}

		namespace MATERIAL {
			namespace ENGINE {
				unsigned int Default();
				unsigned int Outline();
			}
		}
	}

	void LoadBaseAsset();

}