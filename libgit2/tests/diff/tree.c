	cl_git_pass(git_diff_options_init(&opts, GIT_DIFF_OPTIONS_VERSION));
	err = git_error_last();
	cl_assert_equal_i(GIT_ERROR_INVALID, err->klass);
	git_error_clear();
	err = git_error_last();
	cl_git_pass(git_diff_foreach(diff,

void test_diff_tree__diff_tree_with_empty_dir_entry_succeeds(void)
{
	const char *content = "This is a blob\n";
	const git_diff_delta *delta;
	git_oid empty_tree, invalid_tree, blob;
	git_buf patch = GIT_BUF_INIT;
	git_treebuilder *builder;

	g_repo = cl_git_sandbox_init("empty_standard_repo");

	cl_git_pass(git_blob_create_from_buffer(&blob, g_repo, content, strlen(content)));
	cl_git_pass(git_treebuilder_new(&builder, g_repo, NULL));
	cl_git_pass(git_treebuilder_write(&empty_tree, builder));
	cl_git_pass(git_treebuilder_insert(NULL, builder, "empty_tree", &empty_tree, GIT_FILEMODE_TREE));
	cl_git_pass(git_treebuilder_insert(NULL, builder, "blob", &blob, GIT_FILEMODE_BLOB));
	cl_git_pass(git_treebuilder_write(&invalid_tree, builder));

	cl_git_pass(git_tree_lookup(&a, g_repo, &empty_tree));
	cl_git_pass(git_tree_lookup(&b, g_repo, &invalid_tree));
	cl_git_pass(git_diff_tree_to_tree(&diff, g_repo, a, b, NULL));

	cl_git_pass(git_diff_foreach(diff,
		diff_file_cb, diff_binary_cb, diff_hunk_cb, diff_line_cb, &expect));
	cl_assert_equal_i(1, expect.files);
	cl_assert_equal_i(0, expect.file_status[GIT_DELTA_MODIFIED]);
	cl_assert_equal_i(1, expect.hunks);
	cl_assert_equal_i(1, expect.lines);
	cl_assert_equal_i(0, expect.line_ctxt);
	cl_assert_equal_i(1, expect.line_adds);
	cl_assert_equal_i(0, expect.line_dels);

	cl_git_pass(git_diff_to_buf(&patch, diff, GIT_DIFF_FORMAT_PATCH));
	cl_assert_equal_s(patch.ptr,
		"diff --git a/blob b/blob\n"
		"new file mode 100644\n"
		"index 0000000..bbf2e80\n"
		"--- /dev/null\n"
		"+++ b/blob\n"
		"@@ -0,0 +1 @@\n"
		"+This is a blob\n");

	cl_assert_equal_i(git_diff_num_deltas(diff), 1);
	delta = git_diff_get_delta(diff, 0);
	cl_assert_equal_s(delta->new_file.path, "blob");

	git_treebuilder_free(builder);
	git_buf_dispose(&patch);
}