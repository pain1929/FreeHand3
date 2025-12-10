#include "imgui_helper.hpp"
#include "util/ColorH.hpp"

float ImGuiHelper::getWidth() {
	return ImGui::GetContentRegionAvail().x;
}
float ImGuiHelper::getHeight() {
	return ImGui::GetContentRegionAvail().y;
}

ImVec4 ImGuiHelper::rgbaToVec4(float r, float g, float b, float a) {
	return ImVec4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}
ImVec4 ImGuiHelper::rgbaToVec4(ImColor& col) {
	return col.Value;
}

ImVec2 ImGuiHelper::getTextLength(std::string text) {
	return ImGui::CalcTextSize(text.c_str());
}

void ImGuiHelper::renderCombo(std::string title, std::vector<std::string>items, int& index, int comboWidth) {
	ImGui::PushItemWidth(comboWidth);
	if (ImGui::BeginCombo(title.c_str(), items.at(index).c_str(), 0)) {

		for (int n = 0; n < items.size(); n++) {
			const bool is_selected = (index == n);
			if (ImGui::Selectable(items.at(n).c_str(), is_selected))index = n;

			if (is_selected)ImGui::SetItemDefaultFocus();// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
		}
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();
}


void ImGuiHelper::chromaText(std::string text, float sat, float value, float alpha, float offset, float speed, float range) {

	const auto func = [] (const std::string &text) {
		std::vector<std::string> ret;
		for(size_t i = 0; i < text.length();)
		{
			int cplen = 1;
			if((text[i] & 0xf8) == 0xf0) cplen = 4;
			else if((text[i] & 0xf0) == 0xe0) cplen = 3;
			else if((text[i] & 0xe0) == 0xc0) cplen = 2;
			if((i + cplen) > text.length()) cplen = 1;
			ret.push_back(text.substr(i, cplen));
			i += cplen;
		}
		return ret;
	};

	auto vecStr = func(text);
	for (int i= 0; i<vecStr.size();i++) {
		auto& s = vecStr[i];
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(-0.5, 4)); // not the best way, calculating string width will be slightly inaccurate
		float r, g, b;
		ColorH::HSVtoRGB(ColorH::getTimeHue(i * range, speed, offset), sat, value, r, g, b);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(r, g, b, alpha));
		ImGui::Text(s.c_str());
		ImGui::PopStyleColor();

		if (i != vecStr.size() - 1) ImGui::SameLine();
		ImGui::PopStyleVar();
	}
}