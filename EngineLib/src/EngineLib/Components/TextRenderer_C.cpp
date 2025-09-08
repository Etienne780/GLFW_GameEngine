#include <CoreLib/Log.h>
#include <CoreLib/Math.h>

#include "EngineLib/Renderer.h"
#include "EngineLib/GameObject.h"
#include "EngineLib/Components/TextRenderer_C.h"

namespace EngineCore::Component {
	Renderer& TextRenderer::m_renderer = Renderer::GetInstance();
	const std::string compName = "TextRenderer";

	TextRenderer::TextRenderer(GameObjectID gameObjectID) 
		: ComponentBase(compName, gameObjectID) {
		m_gameObject = GetGameObject();
	}

	void TextRenderer::OnInspectorGUIImpl(IUIRenderer& ui) {
		if (m_text.length() < m_visibleChar)
			m_visibleChar = static_cast<int>(m_text.length());
		
		ui.DrawInputText("Text", &m_text);
		ui.DrawDragFloat("Text Size", &m_textSize, 0.075F, 0.1F, 256.0F);
		ui.DrawDragInt("Visible Chars", &m_visibleChar, MathUtil::Min(0.6F, 0.02F * m_text.length()/2), -1, static_cast<int>(m_text.length()));
		ui.DrawColorEdit3("Text Color", &m_textColor);
	}

	void TextRenderer::SubmitDrawCall() {
		
	}

	void TextRenderer::SetText(const std::string& text) {
		m_text = text;

		if (m_text.length() < m_visibleChar)
			m_visibleChar = static_cast<int>(m_text.length());
	}

	void TextRenderer::SetTextColor(float r, float g, float b) {
		m_textColor.Set(r, g, b);
	}

	void TextRenderer::SetTextColor(const Vector3& color) {
		m_textColor = color;
	}

	void TextRenderer::SetTextSize(float textSize) {
		m_textSize = textSize;
	}

	void TextRenderer::SetNumberOfVisibleChar(unsigned int numberOfVisibleChars) {
		m_visibleChar = numberOfVisibleChars;
	}

	const std::string& TextRenderer::GetText() const {
		return m_text;
	}

	const Vector3& TextRenderer::GetTextColor() const {
		return m_textColor;
	}

	float TextRenderer::GetTextSize() const {
		return m_textSize;
	}

	int TextRenderer::GetNumberOfVisibleChar() const {
		return m_visibleChar;
	}


}