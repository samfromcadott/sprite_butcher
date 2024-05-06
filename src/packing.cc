#include <iostream>
#include "finders_interface.h"

export module packing;

using namespace rectpack2D;

constexpr bool allow_flip = true;
const auto runtime_flipping_mode = flipping_option::ENABLED;
export using spaces_type = rectpack2D::empty_spaces<allow_flip, default_empty_spaces>;
export using rect_type = output_rect_t<spaces_type>;
export using rect_array = std::vector<rect_type>;

auto report_successful = [](rect_type&) {
	return callback_result::CONTINUE_PACKING;
};

auto report_unsuccessful = [](rect_type&) {
	return callback_result::ABORT_PACKING;
};

// const auto max_side = 4000;
const auto discard_step = -4;

export std::tuple< rect_wh, rect_array > pack_frames(rect_array rectangles) {
	const int max_side = sqrt( rectangles.size() ) * rectangles[0].w; // TODO: Find the largest side length in rectangles

	const rect_wh canvas_size = find_best_packing<spaces_type>(
		rectangles,
		make_finder_input(
			max_side,
			discard_step,
			report_successful,
			report_unsuccessful,
			runtime_flipping_mode
		)
	);

	return {canvas_size, rectangles};
}
