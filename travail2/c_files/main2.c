#include "../h_files/ft_minishell.h"

#include <stdio.h>
#include <stdlib.h>

// Assuming these are defined elsewhere

void test_command(char *command)
{
    unsigned int i = 0;
    int *token;
    int token_count = 0;
    int expected_count = ft_token_amount(command);
    char    **cmd = ft_create_tokens(command);

    printf("\nTesting command: '%s'\n", command);
    printf("Expected token count: %d\n", expected_count + 1);
    printf("Tokens:\n");

    i = 0;
    while (i < expected_count + 1)
    {
        token = ft_create_types(cmd);
        printf("[%d]: Value: %s Type:'%d'\n", token_count, cmd[i], token[i]);
        token_count++;
        i++;
    }
    printf("Total tokens found: %d\n", token_count);
    if (token_count != expected_count + 1)
        printf("WARNING: Token count mismatch! Expected %d but found %d\n",
               expected_count, token_count);
    printf("----------------------------------------\n");
    i = 0;
    while (cmd[i])
    {
        free(cmd[i]);
        i++;
    }
    free(cmd);
}

int main(void)
{
    char    *line;
    t_data  data;
    t_token tokens;
    char    *env[] = {"PATH=/usr/bin", "CWD=/home/abedin", "LANG=en_US.UTF-8", NULL};

    data.tokens = &tokens;
    data.env = env;
    while (1)
    {
        line = readline("> ");
        data.tokens->name = ft_create_tokens(line);
        data.tokens->type = ft_create_types(data.tokens->name);
        ft_parse2_build_tree(&data);
        ft_parse2_display_cmd_tree(&data, data.root, 0);
        ft_parse3_expand(&data);
        add_history(line);
        free(line);
    }
    return (0);
}
