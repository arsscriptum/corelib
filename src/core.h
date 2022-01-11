
//================================================
// Includes for approximations
//================================================

// #include "approximations\fast_cos.h"
// #include "approximations\fast_float.h"
// #include "approximations\fixed_point.h"
// #include "approximations\simd.h"
// #include "approximations\tables\fastcos_d2.h"
// #include "approximations\tables\fastcos_d3.h"
// #include "approximations\tables\fastcos_d4.h"
// #include "approximations\tables\fastcos_d5.h"
// #include "approximations\tables\fasterfr_d0.h"
// #include "approximations\tables\fasterfr_d1.h"
// #include "approximations\tables\fastexp_limited.h"
// #include "approximations\tables\fastexp2.h"
// #include "approximations\tables\fastgauss_d0.h"
// #include "approximations\tables\fastgauss_d1.h"
// #include "approximations\tables\fastlog2.h"
// #include "approximations\tables\fastlsrgb.h"
// #include "approximations\tables\fastsqrt.h"
// #include "approximations\tables\ffo_lut.h"
// #include "approximations\tables\fpatan2_d2.h"
// #include "approximations\tables\fpcos_d1i.h"
// #include "approximations\tables\fpcos_d2.h"
// #include "approximations\tables\fpdiv_d2.h"
// #include "approximations\tables\fperfr_d0.h"
// #include "approximations\tables\fperfr_d1i.h"
// #include "approximations\tables\fpgauss_d0.h"
// #include "approximations\tables\fpgauss_d1i.h"
// #include "approximations\tables\fracexp2_d1i.h"
// #include "approximations\tables\fraclog2_d1i.h"
// #include "approximations\tables\fracsqrt_d1i.h"

//================================================
// Includes for async_utils
//================================================

// #include "async_utils\AsyncUtils.h"
// #include "async_utils\LockGuard.h"

//================================================
// Includes for console
//================================================

// #include "console\baseterminal.h"
// #include "console\console_colors_define.h"
// #include "console\Console.h"
// #include "console\log.h"
// #include "console\terminal_formatting.h"

//================================================
// Includes for critsec
//================================================

// #include "critsec\critsec.h"

//================================================
// Includes for encrypt
//================================================

// #include "encrypt\base64.h"
// #include "encrypt\blowfish.h"
// #include "encrypt\des.h"
// #include "encrypt\encrypt.h"
// #include "encrypt\MD5.h"
// #include "encrypt\UrlEncode.h"
// #include "encrypt\WinAES.h"

//================================================
// Includes for fileio
//================================================

// #include "fileio\bits.h"
// #include "fileio\dialog.h"
// #include "fileio\dir_explorer_e.h"
// #include "fileio\dir_explorer_t.h"
// #include "fileio\dir_struct.h"
// #include "fileio\dir.h"
// #include "fileio\endian.h"
// #include "fileio\ffmpeg_sound_player.h"
// #include "fileio\file_management.h"
// #include "fileio\file_system.h"
// #include "fileio\file_utilities.h"
// #include "fileio\fileball_struct.h"
// #include "fileio\fileball.h"
// #include "fileio\image_bmp.h"
// #include "fileio\image_fts.h"
// #include "fileio\image_screen.h"
// #include "fileio\image_tiff_lzw.h"
// #include "fileio\image_tiff.h"
// #include "fileio\image.h"
// #include "fileio\open.h"
// #include "fileio\path.h"
// #include "fileio\prefs.h"
// #include "fileio\process.h"
// #include "fileio\sound_aiff.h"
// #include "fileio\sound_format.h"
// #include "fileio\sound_wav.h"
// #include "fileio\tiff_tags.h"

//================================================
// Includes for general
//================================================

// #include "general\audio.h"
// #include "general\base.h"
// #include "general\blocking_queue.h"
// #include "general\crash_dump.h"
// #include "general\enum_to_string.h"
// #include "general\estimates.h"
// #include "general\globals.h"
// #include "general\hashing.h"
// #include "general\intrinsics.h"
// #include "general\keyboard_struct.h"
// #include "general\keyboard.h"
// #include "general\macros.h"
// #include "general\mouse_struct.h"
// #include "general\mouse.h"
// #include "general\noop.h"
// #include "general\structs.h"
// #include "general\textedit_struct.h"
// #include "general\threading.h"
// #include "general\time.h"
// #include "general\valuemap.h"
// #include "general\version.h"
// #include "general\xyz_struct.h"
// #include "general\xyz.h"

//================================================
// Includes for geometry
//================================================

// #include "geometry\bezier.h"
// #include "geometry\distance.h"
// #include "geometry\fit.h"
// #include "geometry\intersection.h"
// #include "geometry\misc.h"
// #include "geometry\rect_struct.h"
// #include "geometry\rect.h"
// #include "geometry\rotation.h"

//================================================
// Includes for graphics
//================================================

// #include "graphics\blending_struct.h"
// #include "graphics\blending.h"
// #include "graphics\blit.h"
// #include "graphics\cl_memory.h"
// #include "graphics\colour.h"
// #include "graphics\draw_effects.h"
// #include "graphics\drawline.h"
// #include "graphics\drawprimitives.h"
// #include "graphics\drawqueue.h"
// #include "graphics\drawqueue\blit.h"
// #include "graphics\drawqueue\drawline.h"
// #include "graphics\drawqueue\drawqueue_enums.h"
// #include "graphics\drawqueue\drawqueue_soft.h"
// #include "graphics\drawqueue\drawrect.h"
// #include "graphics\drawqueue\opencl\drawqueue.cl.h"
// #include "graphics\drawrect.h"
// #include "graphics\graphics_struct.h"
// #include "graphics\graphics.h"
// #include "graphics\mipmap.h"
// #include "graphics\pixel_conv.h"
// #include "graphics\processing.h"
// #include "graphics\sqrgb.h"
// #include "graphics\srgb.h"
// #include "graphics\texture_compression.h"
// #include "graphics\yuv.h"

//================================================
// Includes for gui
//================================================

// #include "gui\control_array.h"
// #include "gui\controls_struct.h"
// #include "gui\controls.h"
// #include "gui\drawelements.h"
// #include "gui\editor_toolbar.h"
// #include "gui\floating_window.h"
// #include "gui\focus_struct.h"
// #include "gui\focus.h"
// #include "gui\inputprocessing_struct.h"
// #include "gui\inputprocessing.h"
// #include "gui\knob_functions.h"
// #include "gui\layout.h"
// #include "gui\make_gui.h"
// #include "gui\positioning.h"
// #include "gui\selection.h"
// #include "gui\visualisations.h"
// #include "gui\window_manager.h"
// #include "gui\zoom_struct.h"
// #include "gui\zoom.h"

//================================================
// Includes for ini
//================================================

// #include "ini\config.h"

//================================================
// Includes for interpreter
//================================================

// #include "interpreter\compilation.h"
// #include "interpreter\decompilation.h"
// #include "interpreter\execution.h"
// #include "interpreter\expression.h"
// #include "interpreter\interpreter.h"
// #include "interpreter\real_functions.h"

//================================================
// Includes for libraries
//================================================

// #include "libraries\cfftpack.h"
// #include "libraries\clfft.h"
// #include "libraries\devil.h"
// #include "libraries\dirent.h"
// #include "libraries\emscripten.h"
// #include "libraries\ffmpeg_enc.h"
// #include "libraries\ffmpeg.h"
// #include "libraries\fftpack.h"
// #include "libraries\glew_minimal.h"
// #include "libraries\libcurl.h"
// #include "libraries\libdr_flac.h"
// #include "libraries\libjpeg.h"
// #include "libraries\libminiaudio.h"
// #include "libraries\libminimp3.h"
// #include "libraries\libraw.h"
// #include "libraries\libsndfile.h"
// #include "libraries\libstb_image.h"
// #include "libraries\libstb_vorbis.h"
// #include "libraries\mpfr.h"
// #include "libraries\openal.h"
// #include "libraries\opencl_struct.h"
// #include "libraries\opencl.h"
// #include "libraries\opencv.h"
// #include "libraries\orig\clew.h"
// #include "libraries\orig\dr_flac.h"
// #include "libraries\orig\fftpack_double.h"
// #include "libraries\orig\fftpack_float.h"
// #include "libraries\orig\miniaudio.h"
// #include "libraries\orig\minimp3.h"
// #include "libraries\orig\miniz.h"
// #include "libraries\orig\stb_image_write.h"
// #include "libraries\orig\stb_image.h"
// #include "libraries\orig\thread.h"
// #include "libraries\orig\tinyexpr.h"
// #include "libraries\orig\xxh3.h"
// #include "libraries\orig\xxhash.h"
// #include "libraries\sdl.h"
// #include "libraries\tinyexpr.h"
// #include "libraries\vulkan_struct.h"
// #include "libraries\vulkan.h"
// #include "libraries\zlib.h"

//================================================
// Includes for math
//================================================

// #include "math\dct.h"
// #include "math\debug.h"
// #include "math\double_double_arithmetic.h"
// #include "math\dsp.h"
// #include "math\erf_radlim.h"
// #include "math\functions.h"
// #include "math\ieee754.h"
// #include "math\matrix.h"
// #include "math\physics.h"
// #include "math\polynomials_mpfr.h"
// #include "math\polynomials.h"
// #include "math\rand.h"

//================================================
// Includes for memory
//================================================

// #include "memory\alloc.h"
// #include "memory\circular_buffer.h"
// #include "memory\comparison.h"
// #include "memory\fill.h"
// #include "memory\generic_buffer.h"
// #include "memory\swap.h"

//================================================
// Includes for pipe
//================================================

// #include "pipe\named_pipe.h"

//================================================
// Includes for regex
//================================================

// #include "regex\regex_helper.h"

//================================================
// Includes for string
//================================================

// #include "string\number_convert.h"
// #include "string\string_utilities.h"
// #include "string\string_utils.h"

//================================================
// Includes for sysInfo
//================================================

// #include "sysInfo\SysInfo.h"

//================================================
// Includes for text
//================================================

// #include "text\edit.h"
// #include "text\format.h"
// #include "text\history.h"
// #include "text\parse.h"
// #include "text\string.h"
// #include "text\undo.h"
// #include "text\unicode_arabic.h"
// #include "text\unicode_bidi.h"
// #include "text\unicode_data_table.h"
// #include "text\unicode_data.h"
// #include "text\unicode.h"

//================================================
// Includes for thread
//================================================

// #include "thread\cthread.h"

//================================================
// Includes for timer
//================================================

// #include "timer\hi_res_timer.h"

//================================================
// Includes for ui
//================================================

// #include "ui\AdvertUI.h"
// #include "ui\ButtonUI.h"
// #include "ui\CalendarUI.h"
// #include "ui\ChooseArea.h"
// #include "ui\CustomControl.h"
// #include "ui\EditUI.h"
// #include "ui\FileDialog.h"
// #include "ui\HintEditUI.h"
// #include "ui\ListExUI.h"
// #include "ui\MsgBox.h"
// #include "ui\MultipleText.h"
// #include "ui\OptionUI.h"
// #include "ui\ScannerEdit.h"
// #include "ui\TabViewV.h"
// #include "ui\TemplateClick.h"

//================================================
// Includes for vector_type
//================================================

// #include "vector_type\cjk.h"
// #include "vector_type\cjkdec_enum.h"
// #include "vector_type\draw.h"
// #include "vector_type\fallback_font.h"
// #include "vector_type\fit.h"
// #include "vector_type\insert_rect.h"
// #include "vector_type\make_font.h"
// #include "vector_type\stats.h"
// #include "vector_type\vector_type_fileball.h"
// #include "vector_type\vector_type_struct.h"
// #include "vector_type\vector_type.h"

//================================================
// Includes for vector
//================================================

// #include "vector\polyline.h"
// #include "vector\vector_struct.h"
// #include "vector\vector.h"

//================================================
// Includes for win
//================================================

// #include "win\exception_catcher.h"
// #include "win\PeVerTool.h"
// #include "win\ScopedHandle.h"
// #include "win\SecurityUtil.h"
// #include "win\SharedMemory.h"
// #include "win\TemplateTimer.h"
// #include "win\win_helpers.h"
// #include "win\windows-api-ex.h"
// #include "win\WinGuiUtil.h"
// #include "win\WinUtil.h"

//================================================
// Includes for xml
//================================================

// #include "xml\tinyxml\tinystr.h"
// #include "xml\tinyxml\tinyxml.h"
// #include "xml\tinyxml\TiXmlDocEx.h"
// #include "xml\tinyxml2.h"
// #include "xml\XmlUtils.h"

//================================================
// Includes for zip
//================================================

// #include "zip\ioapi.h"
// #include "zip\mztools.h"
// #include "zip\unzip.h"
// #include "zip\zconf.h"
// #include "zip\zip.h"
// #include "zip\ZipArchive.h"
// #include "zip\zlib.h"
