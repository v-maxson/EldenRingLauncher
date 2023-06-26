#![windows_subsystem = "windows"]
#![allow(non_camel_case_types)]

mod constants;
mod ui;
mod style;
mod panic_hook;

use glow::HasContext;
use std::ffi::c_void;
use imgui_glow_renderer::AutoRenderer;
use imgui_sdl2_support::*;
use sdl2::{video::Window, event::Event, sys::{SDL_Window, SDL_Point, SDL_HitTestResult}};
use imgui::*;
use constants::*;

fn main() {
    fn glow_context(window: &Window) -> glow::Context {
        unsafe {
            glow::Context::from_loader_function(|s| {
                window.subsystem().gl_get_proc_address(s) as _ 
            })
        }
    }

    unsafe extern "C" fn hit_test(_win: *mut SDL_Window, area: *const SDL_Point, _data: *mut c_void) -> SDL_HitTestResult {
        let area = area.as_ref().unwrap_or_else(|| panic!("hit test error"));
        
        if area.x >= 0 && area.x <= (WINDOW_WIDTH as i32) - 20 && area.y >= 0 && area.y <= 19 {
            return SDL_HitTestResult::SDL_HITTEST_DRAGGABLE
        }

        SDL_HitTestResult::SDL_HITTEST_NORMAL
    }


    let sdl = sdl2::init().unwrap_or_else(|e| panic!("{}", e));
    let video_subsystem = sdl.video().unwrap_or_else(|e| panic!("{}", e));

    

    let gl_attr = video_subsystem.gl_attr();

    gl_attr.set_context_version(3, 3);
    gl_attr.set_context_profile(sdl2::video::GLProfile::Core);

    let window = video_subsystem
        .window(format!("{WINDOW_TITLE} v{VERSION}").as_str(), WINDOW_WIDTH, WINDOW_HEIGHT)
        .allow_highdpi()
        .borderless()
        .opengl()
        .position_centered()
        .build()
        .unwrap_or_else(|e| panic!("{}", e));

    unsafe {
        sdl2::sys::SDL_SetWindowHitTest(
            window.raw(), 
            Some(hit_test), 
            std::ptr::null_mut()
        );
    }


    let gl_context = window.gl_create_context().unwrap_or_else(|e| panic!("{}", e));
    window.gl_make_current(&gl_context).unwrap_or_else(|e| panic!("{}", e));

    let gl = glow_context(&window);

    let mut imgui = Context::create();
    style::apply_custom_style(imgui.style_mut());
    imgui.set_ini_filename(None);
    imgui.set_log_filename(None);

    imgui
        .fonts()
        .add_font(&[imgui::FontSource::TtfData { data: RUDA_BOLD, size_pixels: 14.0, config: None }]);

    let mut platform = SdlPlatform::init(&mut imgui);
    let mut renderer = AutoRenderer::initialize(gl, &mut imgui).unwrap_or_else(|e| panic!("{}", e));

    let mut event_pump = sdl.event_pump().unwrap_or_else(|e| panic!("{}", e));
    let mut running = true;

    'main: loop {
        for event in event_pump.poll_iter() {
            platform.handle_event(&mut imgui, &event);

            if let Event::Quit { .. } = event {
                break 'main;
            }
        }

        /* call prepare_frame before calling imgui.new_frame() */
        platform.prepare_frame(&mut imgui, &window, &event_pump);

        let ui = imgui.new_frame();

        ui::draw_ui(ui, &mut running);

        let draw_data = imgui.render();

        unsafe { renderer.gl_context().clear(glow::COLOR_BUFFER_BIT) };
        renderer.render(draw_data).unwrap_or_else(|e| panic!("{}", e));

        window.gl_swap_window();

        if !running { break; }
    }

}
