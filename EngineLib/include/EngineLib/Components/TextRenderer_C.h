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
			TextRenderer(GameObjectID gameObjectID, FontID id);
			void OnInspectorGUIImpl(IUIRenderer& ui) override;

			COMPONENT_TYPE_DEFINITION(TextRenderer);

			bool IsDrawable() const override { return true; }
			void SubmitDrawCall() override;

			TextRenderer* SetFontID(FontID id);
			TextRenderer* SetText(const std::string& text);
			TextRenderer* SetTextColor(float r, float g, float b);
			TextRenderer* SetTextColor(const Vector3& color);
			TextRenderer* SetTextSize(float textSize);
			TextRenderer* SetTextResolution(int textResolution);
			// -1 = all chars visible
			TextRenderer* SetNumberOfVisibleChar(unsigned int numberOfVisibleChars);

			const std::string& GetText() const;
			const Vector3& GetTextColor() const;
			float GetTextSize() const;
			int GetTextResolution() const;
			int GetNumberOfVisibleChar() const;

		private:
			static Renderer& m_renderer;
			std::shared_ptr<GameObject> m_gameObject = nullptr;
			RenderCommand m_cmd;

			bool m_textChanged = true;
			FontID m_fontID = FontID(0);
			std::string m_text;
			Vector3 m_textColor{ 1, 1, 1 };
			float m_textSize = 5.0f;
			int m_textResolution = 60;
			int m_visibleChar = -1;
			// int m_lastTextSize = 0; could be used in AdjustVisibleChars idk what i want
			std::vector<TextQuad> m_textQuads;

			std::vector<TextQuad>& GetTextQuads();
			void AdjustVisibleChars();
		};

	}

}