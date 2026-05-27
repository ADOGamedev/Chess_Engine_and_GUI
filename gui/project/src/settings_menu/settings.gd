extends Control

@onready var ENGINE_PATH := get_global_path("res://predownloaded_engines/adoce/adoce_v1.0.exe")
@onready var STARTING_FEN := "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

@onready var file_dialog := $file_dialog
@onready var engine_path_line_edit := %path_line_edit
@onready var fen_line_edit := %fen_line_edit
@onready var starting_fen_line_edit := %starting_fen_line_edit

var should_set_game_config = true

func get_global_path(path_res: String) -> String:
	if OS.has_feature("editor"):
		return ProjectSettings.globalize_path(path_res)
	else:
		var base = OS.get_executable_path().get_base_dir()
		var relativa = path_res.replace("res://", "")
		return base + "/" + relativa
		
func _ready() -> void:
	set_starting_fen(STARTING_FEN)
	set_curr_fen(STARTING_FEN)
	engine_path_line_edit.text = ENGINE_PATH
	
	%volume_slider.value = 50

	if !owner:
		return
		
	await owner.ready

	if should_set_game_config:
		owner.set_game_config(engine_path_line_edit.text, starting_fen_line_edit.text)
		should_set_game_config = false
	
func set_curr_fen(fen: String) -> void:
	fen_line_edit.text = fen

func set_starting_fen(fen: String) -> void:
	starting_fen_line_edit.text = fen
	
func _on_apply_button_pressed() -> void:
	visible = false

	if owner and should_set_game_config:
		owner.set_game_config(engine_path_line_edit.text, starting_fen_line_edit.text)

func _on_cancel_button_pressed() -> void:
	visible = false

func _on_path_button_pressed() -> void:
	file_dialog.visible = true

func _on_path_line_edit_text_changed(_new_text: String) -> void:
	should_set_game_config = true

func _on_file_dialog_file_selected(path: String) -> void:
	should_set_game_config = true
	engine_path_line_edit.text = path

func _on_fen_reload_button_pressed() -> void:
	should_set_game_config = true
	starting_fen_line_edit.text = STARTING_FEN

func _on_starting_fen_line_edit_text_changed(_new_text: String) -> void:
	should_set_game_config = true

func _on_volume_slider_value_changed(value: float) -> void:
	var linear_db = value / 100.0
	var db = linear_to_db(linear_db)
	var master_bus_index = AudioServer.get_bus_index("Master")
	AudioServer.set_bus_volume_db(master_bus_index, db)

	%slider_label.text = str(roundi(value)) + "%"


func _on_fen_copy_button_pressed() -> void:
	DisplayServer.clipboard_set(fen_line_edit.text)


