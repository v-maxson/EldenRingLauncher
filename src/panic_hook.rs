use ctor::*;
use crate::constants::*;

#[ctor]
fn panic_hook() {
    std::panic::set_hook(
        Box::new(|info| {
            if let Some(error_message) = info.payload().downcast_ref::<&str>() {
                _ = native_dialog::MessageDialog::new()
                    .set_type(native_dialog::MessageType::Error)
                    .set_text(
                        format!("{} encountered an unexpected error: {}", 
                        WINDOW_TITLE, 
                        error_message
                        ).as_str()
                    )
                    .show_alert();
            } else {
                _ = native_dialog::MessageDialog::new()
                    .set_type(native_dialog::MessageType::Error)
                    .set_text(
                        format!("{} encountered an unexpected error: {}", 
                        WINDOW_TITLE, 
                        info.to_string()
                        ).as_str()
                    )
                    .show_alert();
            }
        })
    );
}