use std::path::Path;

pub fn draw_ui(ui: &mut imgui::Ui, opened: &mut bool) {
    ui.window(format!("{} v{}", crate::WINDOW_TITLE, crate::VERSION).as_str())
        .size([crate::WINDOW_WIDTH as f32, crate::WINDOW_HEIGHT as f32], imgui::Condition::Once)
        .position([0.0, 0.0], imgui::Condition::Once)
        .movable(false)
        .resizable(false)
        .collapsible(false)
        .opened(opened)
        .build(|| {
            if ui.button_with_size("Start Online", [(crate::WINDOW_WIDTH as f32) - 27.0, ((crate::WINDOW_HEIGHT as f32) / 3.0) - 28.0]) {
                let path = Path::new("./start_protected_game_original.exe");

                if !path.exists() {
                    _ = native_dialog::MessageDialog::new()
                        .set_type(native_dialog::MessageType::Error)
                        .set_title("Error!")
                        .set_text("Could not find start_protected_game_original.exe\n\nDid you follow the installation instructions correctly?")
                        .show_alert();

                    return;
                }

                if let Err(e) = std::process::Command::new(path).spawn() {
                    _ = native_dialog::MessageDialog::new()
                        .set_type(native_dialog::MessageType::Error)
                        .set_title("Error!")
                        .set_text(format!("Could not open start_protected_game.exe\n\nError: {}", e).as_str())
                        .show_alert();
                } else {
                    std::process::exit(0);
                }
            }

            ui.separator();

            if ui.button_with_size("Start Seamless Coop", [(crate::WINDOW_WIDTH as f32) - 27.0, ((crate::WINDOW_HEIGHT as f32) / 3.0) - 28.0]) {
                let path = Path::new("./launch_elden_ring_seamlesscoop.exe");

                if !path.exists() {
                    _ = native_dialog::MessageDialog::new()
                        .set_type(native_dialog::MessageType::Error)
                        .set_title("Error!")
                        .set_text("Could not find launch_elden_ring_seamlesscoop.exe\n\nDownload the \"Seamless Coop\" mod from https://www.nexusmods.com/eldenring/mods/510")
                        .show_alert();

                    return;
                }

                if let Err(e) = std::process::Command::new(path).arg("-eac-nop-loaded").spawn() {
                    _ = native_dialog::MessageDialog::new()
                        .set_type(native_dialog::MessageType::Error)
                        .set_title("Error!")
                        .set_text(format!("Could not open launch_elden_ring_seamlesscoop.exe\n\nError: {}", e).as_str())
                        .show_alert();
                } else {
                    std::process::exit(0);
                }
            }

            ui.separator();

            if ui.button_with_size("Start Offline", [(crate::WINDOW_WIDTH as f32) - 27.0, ((crate::WINDOW_HEIGHT as f32) / 3.0) - 28.0]) {
                let path = Path::new("./SignatureMaker.exe");

                if !path.exists() {
                    _ = native_dialog::MessageDialog::new()
                        .set_type(native_dialog::MessageType::Error)
                        .set_title("Error!")
                        .set_text("Could not find eldenring.exe")
                        .show_alert();

                    return;
                }

                if let Err(e) = std::process::Command::new(path).arg("-eac-nop-loaded").spawn() {
                    _ = native_dialog::MessageDialog::new()
                        .set_type(native_dialog::MessageType::Error)
                        .set_title("Error!")
                        .set_text(format!("Could not open eldenring.exe\n\nError: {}", e).as_str())
                        .show_alert();
                } else {
                    std::process::exit(0);
                }
            }
        });
}