#pragma once
#include <string>
#include <CoreLib/Math/Vector4.h>

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
			TextRenderer* SetTextColor(float r, float g, float b, float a);
			TextRenderer* SetTextColor(const Vector4& color);
			TextRenderer* SetTextSize(float textSize);
			TextRenderer* SetTextResolution(int textResolution);
			// -1 = all chars visible
			TextRenderer* SetNumberOfVisibleChar(unsigned int numberOfVisibleChars);
			TextRenderer* SetInvertMesh(bool value);

			const std::string& GetText() const;
			const Vector4& GetTextColor() const;
			float GetTextSize() const;
			int GetTextResolution() const;
			int GetNumberOfVisibleChar() const;
			bool GetInvertMesh() const;

		private:
			static Renderer& m_renderer;
			std::shared_ptr<GameObject> m_gameObject = nullptr;
			RenderCommand m_cmd;

			bool m_textChanged = true;
			FontID m_fontID = FontID(0);
			std::string m_text;
			Vector4 m_textColor{ 1, 1, 1, 1 };
			float m_textSize = 5.0f;
			int m_textResolution = 60;
			int m_visibleChar = -1;
			bool m_invertMesh = false;
			// int m_lastTextSize = 0; could be used in AdjustVisibleChars idk what i want
			std::vector<TextQuad> m_textQuads;

			std::vector<TextQuad>& GetTextQuads();
			void AdjustVisibleChars();
		};

	}

}