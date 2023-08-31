#include "futils.h"
		else if (git_repository__configmap_lookup(&pi->id_strlen, repo, GIT_CONFIGMAP_ABBREV) < 0)
	GIT_ASSERT_ARG(patch);
		git_error_set(GIT_ERROR_PATCH,
	git_buf *out, const git_diff_delta *delta, int id_strlen,
	bool print_index)
		git_error_set(GIT_ERROR_PATCH,
		git_error_set(GIT_ERROR_PATCH,
		if (print_index)
			git_buf_printf(out, "index %s..%s %o\n",
				start_oid, end_oid, delta->old_file.mode);
		if (print_index)
			git_buf_printf(out, "index %s..%s\n", start_oid, end_oid);
	if (git_oid_is_zero(&delta->old_file.id))
	if (git_oid_is_zero(&delta->new_file.id))
static int diff_delta_format_similarity_header(
		git_error_set(GIT_ERROR_PATCH, "invalid similarity %d", delta->similarity);
	GIT_ASSERT(delta->status == GIT_DELTA_RENAMED || delta->status == GIT_DELTA_COPIED);
		type = "copy";
	    (error = git_buf_puts(&new_path, delta->new_file.path)) < 0 ||
	    (error = git_buf_quote(&old_path)) < 0 ||
	    (error = git_buf_quote(&new_path)) < 0)
	git_buf_dispose(&old_path);
	git_buf_dispose(&new_path);
	if (git_oid_is_zero(&delta->old_file.id) &&
		git_oid_is_zero(&delta->new_file.id))
	int id_strlen,
	bool print_index)
	if (unchanged && delta->old_file.mode != delta->new_file.mode)
		diff_print_modes(out, delta);

	    (delta->status == GIT_DELTA_COPIED && unchanged)) {
		if ((error = diff_print_oid_range(out, delta,
						  id_strlen, print_index)) < 0)
	git_buf_dispose(&old_path);
	git_buf_dispose(&new_path);
	if (git_buf_oom(pi->buf))
		return -1;

	if ((error = diff_delta_format_path(&old_path, old_pfx, delta->old_file.path)) < 0 ||
	    (error = diff_delta_format_path(&new_path, new_pfx, delta->new_file.path)) < 0 ||
	    (error = diff_delta_format_with_paths(pi->buf, delta, "Binary files %s and %s differ\n",
						  old_path.ptr, new_path.ptr)) < 0)
	git_buf_dispose(&old_path);
	git_buf_dispose(&new_path);
				   binary->new_file.datalen, binary->new_file.inflatedlen)) < 0 ||
	    (error = format_binary(pi, binary->old_file.type, binary->old_file.data,
				   binary->old_file.datalen, binary->old_file.inflatedlen)) < 0) {
			git_error_clear();
	bool print_index = (pi->format != GIT_DIFF_FORMAT_PATCH_ID);
	    delta->status == GIT_DELTA_UNMODIFIED ||
	    delta->status == GIT_DELTA_IGNORED ||
	    delta->status == GIT_DELTA_UNREADABLE ||
	    (delta->status == GIT_DELTA_UNTRACKED &&
	if ((error = git_diff_delta__format_file_header(pi->buf, delta, oldpfx, newpfx,
							id_strlen, print_index)) < 0)
	case GIT_DIFF_FORMAT_PATCH_ID:
		print_file = diff_print_patch_file;
		print_binary = diff_print_patch_binary;
		print_line = diff_print_patch_line;
		break;
		git_error_set(GIT_ERROR_INVALID, "unknown diff output format (%d)", format);
	if ((error = diff_print_info_init_fromdiff(&pi, &buf, diff, format, print_cb, payload)) < 0)
		goto out;
	if ((error = git_diff_foreach(diff, print_file, print_binary, print_hunk, print_line, &pi)) != 0) {
		git_error_set_after_callback_function(error, "git_diff_print");
		goto out;
out:
	git_buf_dispose(&buf);
		git_error_set(GIT_ERROR_INVALID, "buffer pointer must be provided");
	    line->origin == GIT_DIFF_LINE_DELETION ||
	    line->origin == GIT_DIFF_LINE_CONTEXT)
	int error;
	GIT_UNUSED(delta);
	GIT_UNUSED(hunk);
	    line->origin == GIT_DIFF_LINE_ADDITION ||
	    line->origin == GIT_DIFF_LINE_DELETION) {
		while ((error = fputc(line->origin, fp)) == EINTR)
			continue;
		if (error) {
			git_error_set(GIT_ERROR_OS, "could not write status");
			return -1;
		}
	}

	if (fwrite(line->content, line->content_len, 1, fp) != 1) {
		git_error_set(GIT_ERROR_OS, "could not write line");
		return -1;
	}

	int error;

	GIT_ASSERT_ARG(out);
	GIT_ASSERT_ARG(diff);

	if ((error = git_buf_sanitize(out)) < 0)
		return error;

	return git_diff_print(diff, format, git_diff_print_callback__to_buf, out);
	int error;
	GIT_ASSERT_ARG(patch);
	GIT_ASSERT_ARG(print_cb);
	if ((error = diff_print_info_init_frompatch(&pi, &temp, patch,
						    GIT_DIFF_FORMAT_PATCH, print_cb, payload)) < 0)
		goto out;
	if ((error = git_patch__invoke_callbacks(patch, diff_print_patch_file, diff_print_patch_binary,
						 diff_print_patch_hunk, diff_print_patch_line, &pi)) < 0) {
		git_error_set_after_callback_function(error, "git_patch_print");
		goto out;
out:
	git_buf_dispose(&temp);
	int error;

	GIT_ASSERT_ARG(out);
	GIT_ASSERT_ARG(patch);

	if ((error = git_buf_sanitize(out)) < 0)
		return error;
