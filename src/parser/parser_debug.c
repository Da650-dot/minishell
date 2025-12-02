#include "minishell.h"

/* Debug printing disabled to avoid polluting shell output during normal runs.
 * The functions remain as no-ops so caller sites do not need to be changed.
 */
void	print_cmd(t_cmd *cmd, int cmd_num)
{
	(void)cmd;
	(void)cmd_num;
}

void	print_pipeline(t_pipeline *pipeline)
{
	(void)pipeline;
}

void	test_parser(void)
{
	/* no-op test helper when debug is disabled */
	(void)test_parser;
}
