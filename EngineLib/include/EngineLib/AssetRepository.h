#pragma once

namespace EngineCore {
	
	namespace ID{
		namespace TEXTURE {
			unsigned int No();
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

	void LoadBaseAsset();

}