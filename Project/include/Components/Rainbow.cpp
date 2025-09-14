#include "Rainbow.h"

namespace EngineCore::Component {

	Rainbow::Rainbow(GameObjectID gameObjectID)
		: Script("Rainbow", gameObjectID) {
		
		auto go = GameObject::Get(gameObjectID);
		m_meshRenderer = go->GetComponent<Component::MeshRenderer>();
	}

    void Rainbow::Update() {
        float time = Time::GetTime();

        Vector4 currentColor = m_meshRenderer->GetMeshColor();
        Vector3 hsv = ColorUtils::RGBToHSV(Vector3(currentColor));

        float speed = 72.0f;
        hsv.x = fmod(time * speed, 360.0f);

        hsv.y = 1.0f;
        hsv.z = 1.0f;

        Vector3 rgb = ColorUtils::HSVToRGB(hsv);
        m_meshRenderer->SetMeshColor(Vector4(rgb, 1.0f));
    }

}