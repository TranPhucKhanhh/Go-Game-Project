#include<SFML/Graphics.hpp>
#include<UI/GameOption.h>
#include<core/AssetManager.h>
#include<model/SettingPanel.h>
#include<model/GameState.h>
#include<algorithm>

void GameOption::drawSettingPanel() {
	ui_cfg.window.draw(music_scroll_title);
	ui_cfg.window.draw(effect_scroll_title);
	ui_cfg.window.draw(music_volume_title);
	ui_cfg.window.draw(sound_effect_volume_title);

	background_music_slide.draw(ui_cfg.window);
	sound_effect_slide.draw(ui_cfg.window);
	music_scroll.draw(ui_cfg.window);
	effect_scroll.draw(ui_cfg.window);
}

void GameOption::eventHandleSettingPanel(const sf::Event& event, std::string& respond) {
	std::string event_respond = "";
	
	background_music_slide.eventHandle(event, ui_cfg);
	sound_effect_slide.eventHandle(event, ui_cfg);

	// Update game music & sound according to the scroll
	if (ui_cfg.background_music_volume != background_music_slide.value) {
		ui_cfg.background_music_volume = background_music_slide.value;
		music_volume_title.setString("Music Volume: " + std::to_string((int)ui_cfg.background_music_volume) + "%");
		ui_cfg.music.setVolume(ui_cfg.background_music_volume);
	}
	if (ui_cfg.sound_effect_volume != sound_effect_slide.value) {
		ui_cfg.sound_effect_volume = sound_effect_slide.value;
		sound_effect_volume_title.setString("Sound Effect Volume: " + std::to_string((int)ui_cfg.sound_effect_volume) + "%");
		ui_cfg.updateSoundEffectVolume(ui_cfg.sound_effect_volume);
	}

	music_scroll.eventHandle(event, ui_cfg, event_respond);
	effect_scroll.eventHandle(event, ui_cfg, event_respond);

	if (event_respond.size() > 0 && event_respond[0] == '~') {
		std::string _music_name = event_respond.substr(1);
		if (_music_name == ui_cfg.music_name) return;
		ui_cfg.music_name = _music_name;
		ui_cfg.updateMusic(asset_manager);

		music_scroll_title.setString("Choose your music: " + _music_name);
	}

	if (event_respond.size() > 0 && event_respond[0] == '`') {
		std::string _effect_name = event_respond.substr(1);
		if (_effect_name == ui_cfg.sound_effect_name) return;
		ui_cfg.sound_effect_name = _effect_name;
		ui_cfg.updateSoundEffect(asset_manager);

		effect_scroll_title.setString("In-game sound effect: " + _effect_name);
	}
}

void GameOption::resizeSettingPanel() {
	float _margin = canvas_size.x / 15.f;
	float _inner_margin = _margin / 1.5f;
	float _component_size_y = _margin / 2.f;

	music_volume_title.setCharacterSize(_component_size_y);
	music_volume_title.setPosition({ canvas.getPosition().x - canvas_size.x / 2.f + _margin, new_game_panel.getPos().y + new_game_panel.getSize().y / 2.f + _margin });

	background_music_slide.updateSize({ _component_size_y * 6, _component_size_y });
	background_music_slide.updatePos({ canvas.getPosition().x + canvas_size.x/2.f - _margin - background_music_slide.getSize().x/2.f, music_volume_title.getPosition().y + _component_size_y/2.f});

	sound_effect_volume_title.setCharacterSize(_component_size_y);
	sound_effect_volume_title.setPosition({ canvas.getPosition().x - canvas_size.x / 2.f + _margin, music_volume_title.getPosition().y + _component_size_y + _inner_margin });

	sound_effect_slide.updateSize({ _component_size_y * 6, _component_size_y });
	sound_effect_slide.updatePos({ canvas.getPosition().x + canvas_size.x / 2.f - _margin - sound_effect_slide.getSize().x / 2.f, sound_effect_volume_title.getPosition().y + _component_size_y / 2.f });

	music_scroll_title.setCharacterSize(_component_size_y);
	music_scroll_title.setPosition({ canvas.getPosition().x - canvas_size.x / 2.f + _margin, sound_effect_volume_title.getPosition().y + _component_size_y + _inner_margin });

	effect_scroll_title.setCharacterSize(_component_size_y);
	effect_scroll_title.setPosition({ canvas.getPosition().x - canvas_size.x / 2.f + _margin, music_scroll_title.getPosition().y + _component_size_y + _inner_margin });

	music_scroll.updateSize({ canvas_size.x * 0.4f, canvas_size.x * 0.4f });
	music_scroll.updatePos({ canvas.getPosition().x - canvas_size.x/2.f + music_scroll.getSize().x/2.f + _margin,
		effect_scroll_title.getPosition().y + _margin + music_scroll.getSize().y / 2.f + _component_size_y});

	effect_scroll.updateSize({ canvas_size.x * 0.4f, canvas_size.x * 0.4f });
	effect_scroll.updatePos({ canvas.getPosition().x + canvas_size.x / 2.f - music_scroll.getSize().x / 2.f - _margin, music_scroll.getPos().y});
}