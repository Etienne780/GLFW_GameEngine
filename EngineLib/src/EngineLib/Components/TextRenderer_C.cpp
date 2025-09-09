#include <CoreLib/Log.h>
#include <CoreLib/Math.h>

#include "EngineLib/AssetRepository.h"
#include "EngineLib/FontAsset.h"
#include "EngineLib/FontManager.h"
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

	TextRenderer::TextRenderer(GameObjectID gameObjectID, FontID id)
		: ComponentBase(compName, gameObjectID) {
		m_gameObject = GetGameObject();
		m_fontID = id;
	}

	void TextRenderer::OnInspectorGUIImpl(IUIRenderer& ui) {
		if (m_text.length() < m_visibleChar)
			m_visibleChar = static_cast<int>(m_text.length());
		
		ui.DrawLabel(FormatUtils::formatString("Font ID: {}", (m_fontID.value == ENGINE_INVALID_ID) ? std::string("ENGINE_INVALID_ID") : std::to_string(m_fontID.value).c_str()));
		ui.DrawInputText("Text", &m_text);
		ui.DrawDragFloat("Text Size", &m_textSize, 0.075F, 1, 256);
		ui.DrawDragInt("Text Reselution", &m_textResolution, 0.075F, 0.1f);
		ui.DrawDragInt("Visible Chars", &m_visibleChar, MathUtil::Min(0.6F, 0.02F * m_text.length()/2), -1, static_cast<int>(m_text.length()));
		ui.DrawColorEdit3("Text Color", &m_textColor);

		m_textChanged = true;
	}
	
	void TextRenderer::SubmitDrawCall() {
		if (m_fontID.value == ENGINE_INVALID_ID)
			return;

		m_cmd.type = RenderCommandType::Text;
		m_cmd.materialID = ASSETS::ENGINE::MATERIAL::DefaultText();
		m_cmd.renderLayer = m_gameObject->GetRenderLayer();
		m_cmd.modelMatrix = m_gameObject->GetTransform()->GetWorldModelMatrixPtr();
		m_cmd.fontID = m_fontID;
		m_cmd.pixelSize = m_textResolution;
		m_cmd.textColor = m_textColor;
		m_cmd.textQuads = GetTextQuads();

		m_renderer.Submit(m_cmd);
	}

	TextRenderer* TextRenderer::SetFontID(FontID id) {
		m_textChanged = true;
		m_fontID = id;

		return this;
	}

	TextRenderer* TextRenderer::SetText(const std::string& text) {
		m_textChanged = true;
		m_text = text;

		if (m_text.length() < m_visibleChar)
			m_visibleChar = static_cast<int>(m_text.length());

		return this;
	}

	TextRenderer* TextRenderer::SetTextColor(float r, float g, float b) {
		m_textChanged = true;
		m_textColor.Set(r, g, b);

		return this;
	}

	TextRenderer* TextRenderer::SetTextColor(const Vector3& color) {
		m_textChanged = true;
		m_textColor = color;

		return this;
	}

	TextRenderer* TextRenderer::SetTextSize(int textSize) {
		m_textChanged = true;
		m_textSize = textSize;

		return this;
	}

	TextRenderer* TextRenderer::SetTextResolution(int textResolution) {
		m_textChanged = true;
		m_textResolution = textResolution;

		return this;
	}

	TextRenderer* TextRenderer::SetNumberOfVisibleChar(unsigned int numberOfVisibleChars) {
		m_textChanged = true;
		m_visibleChar = numberOfVisibleChars;

		return this;
	}

	const std::string& TextRenderer::GetText() const {
		return m_text;
	}

	const Vector3& TextRenderer::GetTextColor() const {
		return m_textColor;
	}

	int TextRenderer::GetTextSize() const {
		return m_textSize;
	}

	int TextRenderer::GetNumberOfVisibleChar() const {
		return m_visibleChar;
	}

	std::vector<TextQuad>& TextRenderer::GetTextQuads() {
		if (m_textChanged) {
			float textSize = static_cast<float>(m_textSize / m_textResolution);
			auto allQuads = FontManager::BuildTextQuads(m_text, m_fontID, { 0.0f, 0.0f }, m_textResolution, textSize);

			if (m_visibleChar == -1 || m_visibleChar >= static_cast<int>(allQuads.size())) {
				m_textQuads = std::move(allQuads);
			}
			else {
				m_textQuads.assign(allQuads.begin(), allQuads.begin() + m_visibleChar);
			}

			m_textChanged = false;
		}
		return m_textQuads;
	}

}