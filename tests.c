#include "minishell.h"

#include <assert.h>

#include <criterion/criterion.h>
#include <criterion/new/assert.h>


Test(input_is_space, simple_tests)
{
	assert( input_is_space("     "));
	assert( ! input_is_space("  abc  "));
}


Test(tokenization, quotes) {
	t_global global;
	global.token = NULL;
	global.env = NULL;
	global.user_input = "\'abc";
	assert( tokenization(&global) == 1);
}


Test(tokenization, three_tokens) {
	t_global global;
	global.token = NULL;
	global.env = NULL;

	global.user_input = "abc    def      xyz";
	assert( tokenization(&global) == 0);

	t_token* temp = global.token;
	assert ( ft_strncmp(temp->str, "abc", ft_strlen(temp->str)) == 0 );
	assert ( temp->type == WORD );
	temp = temp->next;

	assert ( ft_strncmp(temp->str, "def", ft_strlen(temp->str)) == 0 );
	assert ( temp->type == WORD );
	temp = temp->next;

	assert ( ft_strncmp(temp->str, "xyz", ft_strlen(temp->str)) == 0 );
	assert ( temp->type == WORD );
	temp = temp->next;

	assert(temp->type == END);

}
// GENERATED BY CHATGPT
// and updated


t_token *create_token(char *str, int type) {
    t_token *new_token = malloc(sizeof(t_token));
    new_token->str = strdup(str);
    new_token->type = type;
    new_token->status = 0;
    new_token->prev = NULL;
    new_token->next = NULL;
    return new_token;
}

void free_tokens(t_token *head) {
    t_token *temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp->str);
        free(temp);
    }
}

Test(check_var, pipe_at_beginning) {
    t_token *head = create_token("|", PIPE);
    cr_expect(check_var(&head) == 1, "Expected syntax error for pipe at beginning");
    free_tokens(head);
}

Test(check_var, variable_detection) {
    t_token *head = create_token("$VAR", 0);
    cr_expect(check_var(&head) == 0, "Expected no error for variable token");
    cr_expect(head->type == VAR, "Expected token type to be VAR");
    free_tokens(head);
}

Test(check_var, consecutive_pipes) {
    t_token * head = create_token("abc", WORD);
    t_token *token1 = create_token("|", PIPE);
    t_token *token2 = create_token("|", PIPE);
    t_token *token3 = create_token("def", WORD);

    head->next = token1;
    token1->prev = head;

    token1->next = token2;
    token2->prev = token1;

    token2->next = token3;
    token3->prev = token2;
    cr_expect(check_var(&head) == 1, "Expected syntax error for consecutive pipes");
    free_tokens(head);
}

Test(check_var, mixed_valid_tokens) {
    t_token *head = create_token("echo", WORD);
    head->next = create_token("$HOME", VAR);
    head->next->prev = head;
    cr_expect(check_var(&head) == 0, "Expected no error for valid tokens");
    free_tokens(head);
}


// var_expander


// Helper function to create a mock global env
t_global *create_mock_global(char **env) {
    t_global *global = malloc(sizeof(t_global));
    global->env = env;
    return global;
}

// Test variable expansion
Test(var_expander, expands_simple_variable) {
    char *env[] = {"USER=testuser", "HOME=/home/testuser", NULL};
    t_global *global = create_mock_global(env);
    t_token *token = create_token("$USER", VAR);
    
    var_expander(global, &token);
    
    cr_assert_str_eq(token->str, "testuser", "Expected USER to expand to testuser");
    free(token->str);
    free(token);
    free(global);
}

// Test expansion of $? special variable
Test(var_expander, expands_exit_code) {
    char *env[] = {NULL};
    t_global *global = create_mock_global(env);
    g_last_exit_code = 42;
    t_token *token = create_token("$?", VAR);
    
    var_expander(global, &token);
    
    cr_assert_str_eq(token->str, "42", "Expected $? to expand to 42");
    free(token->str);
    free(token);
    free(global);
}

// Test handling of non-existent variable
Test(var_expander, non_existent_variable) {
    char *env[] = {"USER=testuser", NULL};
    t_global *global = create_mock_global(env);
    t_token *token = create_token("$FOO", VAR);
    
    var_expander(global, &token);
    
    cr_assert_str_eq(token->str, "", "Expected non-existent variable to be removed");
    free(token->str);
    free(token);
    free(global);
}

// Test variable expansion inside quotes
Test(var_expander, expands_within_double_quotes) {
    char *env[] = {"USER=testuser", NULL};
    t_global *global = create_mock_global(env);
    t_token *token = create_token("\"$USER\"", VAR);
    
    var_expander(global, &token);
    
    cr_assert_str_eq(token->str, "\"testuser\"", "Expected expansion inside double quotes");
    free(token->str);
    free(token);
    free(global);
}

// Test variable not expanding inside single quotes
Test(var_expander, does_not_expand_within_single_quotes) {
    char *env[] = {"USER=testuser", NULL};
    t_global *global = create_mock_global(env);
    t_token *head = create_token("echo", WORD);
    t_token *token = create_token("'$USER'", VAR);
    
    head->next = token;
    token->prev = head;

    var_expander(global, &head);
    
    cr_assert_str_eq(head->str, "echo");
    cr_assert_str_eq(token->str, "'$USER'", "Expected no expansion inside single quotes");
    free(head->str);
    free(head);
    free(token);
    free(global);
}

// quotes

Test(handle_quotes, remove_single_quotes) {
	t_token *head = create_token("'hello'", WORD);
	t_global global = {.token = head};

	handle_quotes(&global);

	assert(strcmp(head->str, "hello") == 0);
	assert(head->join);

	free_tokens(head);
}

Test(handle_quotes, remove_double_quotes) {
	t_token *head = create_token("\"world\"", WORD);
	t_global global = {.token = head};

	handle_quotes(&global);

	assert(strcmp(head->str, "world") == 0);
	assert(head->join);

	free_tokens(head);
}

Test(handle_quotes, mixed_quotes_inside_string) {
	t_token *head = create_token("The 'quick' brown \"fox\"", WORD);
	t_global global = {.token = head};

	handle_quotes(&global);

	assert(strcmp(head->str, "The quick brown fox") == 0);
	assert(head->join);

	free_tokens(head);
}

Test(handle_quotes, no_quotes_should_remain_unchanged) {
	t_token *head = create_token("justtext", WORD);
	t_global global = {.token = head};

	handle_quotes(&global);

	assert(strcmp(head->str, "justtext") == 0);
	assert(! head->join);

	free_tokens(head);
}

Test(handle_quotes, heredoc_should_not_remove_quotes) {
	t_token *heredoc = create_token("<<", HEREDOC);
	t_token *quoted_token = create_token("'do_not_remove'", WORD);

	heredoc->next = quoted_token;
	quoted_token->prev = heredoc;

	t_global global = {.token = heredoc};

	handle_quotes(&global);

	assert(strcmp(quoted_token->str, "'do_not_remove'") == 0);
	assert(! quoted_token->join);

	free_tokens(heredoc);
}

Test(handle_quotes, multiple_tokens_mixed) {
	t_token *t1 = create_token("'alpha'", WORD);
	t_token *t2 = create_token("\"beta\"", WORD);
	t_token *t3 = create_token("gamma", WORD);

	t1->next = t2; t2->prev = t1;
	t2->next = t3; t3->prev = t2;

	t_global global = {.token = t1};

	handle_quotes(&global);

	assert(strcmp(t1->str, "alpha") == 0);
	assert(strcmp(t2->str, "beta") == 0);
	assert(strcmp(t3->str, "gamma") == 0);
	assert(t1->join);
	assert(t2->join);
	assert(! t3->join);

	free_tokens(t1);
}



// MOCK HELPERS (should match your actual structures)

void link_tokens(t_token *a, t_token *b) {
    a->next = b;
    b->prev = a;
}



// ===================== TESTS ===================== //

Test(parse_word, handles_single_word) {
    t_token *tok = create_token("echo", WORD);
    t_token *end = create_token("", END);
    link_tokens(tok, end);
    t_command *cmd = lst_new_cmd(false);

    parse_word(&cmd, &tok);
    // cr_assert_str_eq(cmd->command, "echo");
}

Test(parse_input, parses_input_redirection) {
    t_token *redir = create_token("<", INPUT);
    t_token *filename = create_token("input.txt", WORD);
    link_tokens(redir, filename);

    t_command *cmd = lst_new_cmd(false);
    t_token *tok = redir;

    parse_input(&cmd, &tok);
    cr_assert_str_eq(cmd->io_fds->infile, "input.txt");
}

Test(parse_trunc, parses_output_trunc_redirection) {
    t_token *redir = create_token(">", TRUNC);
    t_token *filename = create_token("/tmp/out.txt", WORD);
    link_tokens(redir, filename);

    t_command *cmd = lst_new_cmd(false);
    t_token *tok = redir;

    parse_trunc(&cmd, &tok);
    cr_assert_str_eq(cmd->io_fds->outfile, "/tmp/out.txt");
}

Test(parse_append, parses_output_append_redirection) {
    t_token *redir = create_token(">>", APPEND);
    t_token *filename = create_token("/tmp/log.txt", WORD);
    link_tokens(redir, filename);

    t_command *cmd = lst_new_cmd(false);
    t_token *tok = redir;

    parse_append(&cmd, &tok);
    cr_assert_str_eq(cmd->io_fds->outfile, "/tmp/log.txt");
}

Test(parse_pipe, creates_next_command) {
    t_token *tok = create_token("|", PIPE);
    t_command *cmd = lst_new_cmd(false);

    parse_pipe(&cmd, &tok);

    cr_assert_not_null(cmd->next);
    cr_assert(cmd->pipe_output);
}

// TODO fix this test case with correct tokens for heredoc
// Test(parse_heredoc, sets_heredoc_delimiter_and_infile) {
//     t_token *redir = create_token("<<", HEREDOC);
//     t_token *delim = create_token("EOF", WORD);
//     t_token *delim_end = create_token("EOF", WORD);
//     t_token *end = create_token("", END);
//     link_tokens(redir, delim);
//     link_tokens(delim, delim_end);
//     link_tokens(delim_end, end);

//     t_command *cmd = lst_new_cmd(false);
//     t_token *tok = redir;
//     t_global g;
//     char *env[] = {NULL};
//     init_global(&g, env);

//     parse_heredoc(&g, &cmd, &tok);
//     cr_assert_not_null(cmd->io_fds->infile);
//     cr_assert_str_eq(cmd->io_fds->heredoc_delimiter, "EOF");
// }

Test(create_commands, creates_basic_command_structure) {
    // Simulate: echo hello | grep test > out.txt
    t_token *tok1 = create_token("echo", WORD);
    t_token *tok2 = create_token("hello", WORD);
    t_token *tok3 = create_token("|", PIPE);
    t_token *tok4 = create_token("grep", WORD);
    t_token *tok5 = create_token("test", WORD);
    t_token *tok6 = create_token(">", TRUNC);
    t_token *tok7 = create_token("/tmp/out.txt", WORD);

    link_tokens(tok1, tok2); link_tokens(tok2, tok3);
    link_tokens(tok3, tok4); link_tokens(tok4, tok5);
    link_tokens(tok5, tok6); link_tokens(tok6, tok7);

    t_global g;
    char *env[] = {NULL};
    init_global(&g, env);
    g.token = tok1;

    create_commands(&g, g.token);

    cr_assert_str_eq(g.cmd->command, "echo");
    cr_assert_str_eq(g.cmd->next->command, "grep");
    cr_assert_str_eq(g.cmd->next->io_fds->outfile, "/tmp/out.txt");
}