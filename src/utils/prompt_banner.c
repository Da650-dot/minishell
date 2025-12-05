/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_banner.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgiancol <jgiancol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 06:58:53 by jgiancol          #+#    #+#             */
/*   Updated: 2025/12/05 06:59:21 by jgiancol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_welcome_banner(void)
{
	printf("\n");
	printf("╔══════════════════════════════════════════════════╗\n");
	printf("║             🐚  MINISHELL  🐚                    ║\n");
	printf("║          Bem-vindo ao seu shell!                ║\n");
	printf("╚══════════════════════════════════════════════════╝\n");
	printf("\n");
	printf("  Digite 'exit' ou pressione Ctrl-D para sair\n");
	printf("\n");
}

void	clear_screen(void)
{
	printf("\033[H\033[J");
}

void	print_exit_message(void)
{
	printf("\n");
	printf("👋 Até logo! Obrigado por usar o minishell.\n");
	printf("\n");
}
