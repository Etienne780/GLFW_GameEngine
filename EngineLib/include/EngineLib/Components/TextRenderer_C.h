#pragma once
#include <string>
#include <CoreLib/Math/Vector3.h>

#include "../ComponentBase.h"
#include "../EngineTypes.h"

namespace EngineCore {
	class Renderer;

	namespace Component {

		class TextRenderer : public EngineCore::ComponentBase {
		public:
			TextRenderer(GameObjectID gameObjectID);
			void OnInspectorGUIImpl(IUIRenderer& ui) override;

			COMPONENT_TYPE_DEFINITION(TextRenderer);

			bool IsDrawable() const override { return true; }
			void SubmitDrawCall() override;

			void SetText(const std::string& text);
			void SetTextColor(float r, float g, float b);
			void SetTextColor(const Vector3& color);
			void SetTextSize(float textSize);
			// -1 = all chars visible
			void SetNumberOfVisibleChar(unsigned int numberOfVisibleChars);

			const std::string& GetText() const;
			const Vector3& GetTextColor() const;
			float GetTextSize() const;
			int GetNumberOfVisibleChar() const;

		private:
			static Renderer& m_renderer;
			std::shared_ptr<GameObject> m_gameObject = nullptr;
			RenderCommand m_cmd;

			std::string m_text;
			Vector3 m_textColor;
			float m_textSize = 24;
			int m_visibleChar = -1;
		};

	}

}