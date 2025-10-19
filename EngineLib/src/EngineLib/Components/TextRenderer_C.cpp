#include <CoreLib/Log.h>
#include <CoreLib/Math.h>

#include "EngineLib/AssetRepository.h"
#include "EngineLib/FontAsset.h"
#include "EngineLib/FontManager.h"
#include "EngineLib/Renderer.h"
#include "EngineLib/GameObject.h"
#include "EngineLib/Components/TextRenderer_C.h"

namespace EngineCore::Component {

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

	void TextRenderer::OnInspectorGUIImpl(IComponentUIRenderer& ui) {		
		ui.DrawLabel(FormatUtils::formatString("Font ID: {}", (m_fontID.value == ENGINE_INVALID_ID) ? std::string("ENGINE_INVALID_ID") : std::to_string(m_fontID.value).c_str()));
		ui.DrawInputText("Text", &m_text);
		ui.DrawDragFloat("Text Size", &m_textSize, 0.075F, 1, 256);
		ui.DrawDragInt("Text Reselution", &m_textResolution, 0.075F, 1);
		ui.DrawDragInt("Visible Chars", &m_visibleChar, MathUtil::Min(0.6F, 0.02F * m_text.length()/2), -1, static_cast<int>(m_text.length()));
		ui.DrawColorEdit4("Text Color", &m_textColor);
		ui.DrawCheckbox("Invert Mesh", &m_invertMesh);

		m_textSize = abs(MathUtil::Max(1, m_textSize));
		m_textResolution = abs(m_textResolution);
		AdjustVisibleChars();

		m_textChanged = true;
	}
	
	void TextRenderer::SubmitDrawCall() {
		if (m_fontID.value == ENGINE_INVALID_ID)
			return;

		m_cmd.type = RenderCommandType::Text;
		m_cmd.invertMesh = m_invertMesh;
		m_cmd.materialID = ASSETS::ENGINE::MATERIAL::DefaultText();
		m_cmd.renderLayerID = m_gameObject->GetRenderLayer();
		m_cmd.modelMatrix = m_gameObject->GetTransform()->GetWorldModelMatrixPtr();
		m_cmd.fontID = m_fontID;
		m_cmd.pixelSize = m_textResolution;
		m_cmd.textQuads = GetTextQuads();
		m_cmd.meshColor = m_textColor;
		m_cmd.isTransparent = true;

		m_renderer->Submit(m_cmd);
	}

	TextRenderer* TextRenderer::SetFontID(FontID id) {
		if (IsDead("Cant set FontID")) {
			return this;
		}
		m_textChanged = true;
		m_fontID = id;

		return this;
	}

	TextRenderer* TextRenderer::SetText(const std::string& text) {
		if (IsDead("Cant set text")) {
			return this;
		}
		m_textChanged = true;
		m_text = text;
		AdjustVisibleChars();

		return this;
	}

	TextRenderer* TextRenderer::SetTextColor(float r, float g, float b, float a) {
		if (IsDead("Cant set text color")) {
			return this;
		}
		m_textChanged = true;
		m_textColor.Set(r, g, b, a);

		return this;
	}

	TextRenderer* TextRenderer::SetTextColor(const Vector4& color) {
		if (IsDead("Cant set text color")) {
			return this;
		}
		m_textChanged = true;
		m_textColor = color;

		return this;
	}

	TextRenderer* TextRenderer::SetTextSize(float textSize) {
		if (IsDead("Cant set text size")) {
			return this;
		}
		m_textChanged = true;
		m_textSize = textSize;
		m_textSize = abs(MathUtil::Max(1, m_textSize));

		return this;
	}

	TextRenderer* TextRenderer::SetTextResolution(int textResolution) {
		if (IsDead("Cant set text resolution")) {
			return this;
		}
		m_textChanged = true;
		m_textResolution = textResolution;
		m_textResolution = abs(m_textResolution);

		return this;
	}

	TextRenderer* TextRenderer::SetNumberOfVisibleChars(unsigned int numberOfVisibleChars) {
		if (IsDead("Cant set Number of visible char")) {
			return this;
		}
		m_textChanged = true;
		m_visibleChar = numberOfVisibleChars;

		return this;
	}

	TextRenderer* TextRenderer::SetInvertMesh(bool value) {
		if (IsDead("Cant set invert mesh")) {
			return this;
		}
		m_invertMesh = value;
		return this;
	}

	const std::string& TextRenderer::GetText() const {
		return m_text;
	}

	const Vector4& TextRenderer::GetTextColor() const {
		if (IsDead("Cant get text color")) {
			static Vector4 dummy;
			return dummy;
		}
		return m_textColor;
	}

	float TextRenderer::GetTextSize() const {
		if (IsDead("Cant get text size")) {
			return 0.0f;
		}
		return m_textSize;
	}

	int TextRenderer::GetTextResolution() const {
		if (IsDead("Cant get text resolution")) {
			return 0;
		}
		return m_textResolution;
	}

	int TextRenderer::GetNumberOfVisibleChars() const {
		if (IsDead("Cant get number of visible chars")) {
			return 0;
		}
		return m_visibleChar;
	}

	bool TextRenderer::GetInvertMesh() const {
		if (IsDead("Cant get invert mesh")) {
			return false;
		}
		return m_invertMesh;
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

	void TextRenderer::AdjustVisibleChars() {
		/*
		* Code to to adjust the number of visible chars
		int currentTextSize = static_cast<int>(m_text.length());
		// int diff = currentTextSize - m_lastTextSize;

		if (m_visibleChar != -1) {
			// if (diff > 0) {
			// 	// moves visible chars when new char added
			// 	// posi (+)
			// 	m_visibleChar += diff;
			// } diff < 0 && 
			if (m_visibleChar >= currentTextSize) {
				// nege (-)
				m_visibleChar = currentTextSize;
			}
		}*/

		// m_lastTextSize = currentTextSize;
	}

}