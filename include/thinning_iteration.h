#ifndef THEBE_THINNING_ITERATION
#define THEBE_THINNING_ITERATION

#define THINNING_ITERATION_OLD(N)                                             \
	static bool thinning_iteration_##N(cv::Mat1b& img, cv::Mat1b& out) {	  \
																			  \
        out.setTo(0);                                                         \
                                                                              \
        uint16_t block;                                                       \
        bool modified = false;                                                \
        for (int r = 1; r < img.rows - 1; r++) {                              \
			block = 0;                                                        \
            block |= img(r - 1, 0) << 2;                                      \
            block |= img(r, 0) << 5;                                          \
            block |= img(r + 1, 0) << 8;                                      \
            for (int c = 0; c < img.cols - 1; c++) {                          \
                block >>= 1;                                                  \
                block = block & ~(1 << 2) | (img(r - 1, c + 1) << 2);         \
                block = block & ~(1 << 5) | (img(r, c + 1) << 5);             \
                block = block & ~(1 << 8) | (img(r + 1, c + 1) << 8);         \
                                                                              \
                if (!(block & (1 << 4)))                                      \
                    continue;                                                 \
                                                                              \
                if (!should_remove_##N(block))                                \
                    out(r, c) = 1;                                            \
                else                                                          \
                    modified = true;                                          \
            }                                                                 \
            /* Last column */                                                 \
            block >>= 1;                                                      \
            block = block & ~(1 << 2);                                        \
            block = block & ~(1 << 5);                                        \
            block = block & ~(1 << 8);                                        \
            if (!(block & (1 << 4)))                                          \
                continue;                                                     \
            if (!should_remove_##N(block))                                    \
                out(r, img.cols - 1) = 1;                                     \
            else                                                              \
                modified = true;                                              \
        }                                                                     \
                                                                              \
        return modified;                                                      \
    }

#define THINNING_ITERATION(N)                                                 \
	static bool thinning_iteration_##N(cv::Mat1b& img, cv::Mat1b& out) {	  \
																			  \
        out.setTo(0);                                                         \
                                                                              \
        uint16_t block;                                                       \
        bool modified = false;                                                \
        for (int r = 1; r < img.rows - 1; r++) {                              \
			const unsigned char* const img_row = img.ptr<unsigned char>(r);   \
			const unsigned char* const img_row_prev = (unsigned char *)(((char *)img_row) - img.step.p[0]); \
			const unsigned char* const img_row_foll = (unsigned char *)(((char *)img_row) + img.step.p[0]);	\
			unsigned char* const out_row = out.ptr<unsigned char>(r);		  \
			block = 0;                                                        \
            block |= img_row_prev[0] << 2;                                    \
            block |= img_row[0] << 5;							              \
            block |= img_row_foll[0] << 8;                                    \
            for (int c = 0; c < img.cols - 1; c++) {                          \
                block >>= 1;                                                  \
                block = block & ~(1 << 2) | (img_row_prev[c + 1] << 2);       \
                block = block & ~(1 << 5) | (img_row[c + 1] << 5);            \
                block = block & ~(1 << 8) | (img_row_foll[c + 1] << 8);       \
                                                                              \
                if (!(block & (1 << 4)))                                      \
                    continue;                                                 \
                                                                              \
                if (!should_remove_##N(block))                                \
                    out_row[c] = 1;                                           \
                else                                                          \
                    modified = true;                                          \
            }                                                                 \
            /* Last column */                                                 \
            block >>= 1;                                                      \
            block = block & ~(1 << 2);                                        \
            block = block & ~(1 << 5);                                        \
            block = block & ~(1 << 8);                                        \
            if (!(block & (1 << 4)))                                          \
                continue;                                                     \
            if (!should_remove_##N(block))                                    \
                out_row[img.cols - 1] = 1;                                    \
            else                                                              \
                modified = true;                                              \
        }                                                                     \
                                                                              \
        return modified;                                                      \
    }

#define PERFORM_THINNING void PerformThinning()                               \
{                                                                             \
    /*The input image should be binary (0 background, 255 foreground)*/       \
    cv::Mat1b img_input = img_.clone()/255;                                   \
    img_out_ = cv::Mat1b(img_.size());                                        \
                                                                              \
    bool i0, i1;                                                              \
    do {                                                                      \
        i0 = thinning_iteration_0(img_input, img_out_);                       \
        i1 = thinning_iteration_1(img_out_, img_input);                       \
    } while (i0 || i1);                                                       \
                                                                              \
    img_out_ *= 255;                                                          \
}

#define PERFORM_THINNING_DT void PerformThinning()                             \
{                                                                              \
    /* The input image should be binary (0 background, 255 foreground)*/       \
    img_out_ = img_.clone() / 255;                                             \
                                                                               \
    bool i0, i1;                                                               \
    do {                                                                       \
        i0 = thinning_iteration(img_out_, 0);                                  \
        i1 = thinning_iteration(img_out_, 1);                                  \
    } while (i0 || i1);                                                        \
                                                                               \
    img_out_ *= 255;                                                           \
}

#endif // !THEBE_THINNING_ITERATION
