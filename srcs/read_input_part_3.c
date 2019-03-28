/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input_part_3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsimonis <tsimonis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/02 10:41:48 by bsprigga          #+#    #+#             */
/*   Updated: 2019/03/29 02:56:27 by tsimonis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

static void	add_to_lst(t_room *input, t_room *output)
{
	t_neighbour	*neighb;

	if (!(neighb = (t_neighbour *)malloc(sizeof(*neighb))))
		exit(0);
	neighb->room = input;
	if (output->neighbours)
		neighb->next = output->neighbours;
	else
		neighb->next = NULL;
	output->neighbours = neighb;
}

static int	link_writing_main_loop(char *line, char **line_split)
{
	char	*first_name;
	char	*second_name;
	t_room	*tmps[2];
	size_t	i;

	i = 0;
	while (i < ft_arrlen(line_split) - 1)
	{
		if (!(first_name = ft_strjoin_for_arr(line_split, i + 1)) ||
			!(second_name = ft_strdup(line + ft_strlen(first_name) + 1)))
			exit(0);
		if (++i && (tmps[0] = find_leaf(first_name))
				&& (tmps[1] = find_leaf(second_name)))
			break ;
		free(first_name);
		free(second_name);
	}
	if (!(tmps[0]) || !(tmps[1]) || !first_name || !second_name)
		//error_exit(e_invalid_link); // need to rework on check_data_sufficiency
		return (1);
	add_to_lst(tmps[1], tmps[0]);
	add_to_lst(tmps[0], tmps[1]);
	free_2d_array(line_split);
	free(first_name);
	free(second_name);
	return (0);
}

int			link_writing(char **line)
{
	char		**line_split;

	if (!(g_params->arr))
		sort_list_to_arr();
	line_split = ft_strsplit(*line, '-');
	if (ft_arrlen(line_split) < 2) // why < ? not != because we want to parse a-b-c-d-...
		// error_exit(e_invalid_link); // need to rework on check_data_sufficiency
		return (1);
	if (link_writing_main_loop(*line, line_split))
		return (1);
	return (0);
}

void		check_data_sufficiency(void)
{
	if (!(g_params->nr_ants))
		error_exit(e_no_ants_value);
	else if (!(g_params->start))
		error_exit(e_no_start_node);
	else if (!(g_params->end))
		error_exit(e_no_end_node);
}

/*
**	fls[0] -- was there a start?
**	fls[1] -- was there an end?
**	fls[2] -- was there a link?
*/

static int	read_input_loop_conditions(int *fls, char *line,
									char **line_split, t_list **input)
{
	int		stop_reading;

	stop_reading = 0;
	if (line[0] == '#' && !ft_strequ(line, "##start")
			&& !ft_strequ(line, "##end"))
		;
	else if ((ft_strequ(line, "##start") && (fls[0] = 1))
			|| (ft_strequ(line, "##end") && (fls[1] = 1)))
		start_end_writing(&line, input);
	else if (ft_arrlen(line_split) == 3 && !(fls[2]))
		room_writing(line_split);
	else if (ft_arrlen(line_split) == 1 && (fls[2] = 1))
		stop_reading = link_writing(&line);
	else
	{
		check_data_sufficiency();
		stop_reading = 1;
	}
	ft_lstadd(input, ft_lstnew(line, 0));
	g_params->read_lines++;
	return (stop_reading);
}

void		read_input(t_list **input)
{
	char	*line;
	char	**line_split;
	int		fls[3];
	int		stop_reading;

	stop_reading = 0;
	g_params_init(&fls, &line);
	get_nr_ants(&line, input);
	while (get_next_line_or_exit(&line) > 0 && !stop_reading)
	{
		line_split = ft_strsplit(line, ' ');
		stop_reading = read_input_loop_conditions(fls, line, line_split, input);
		free_2d_array(line_split);
	}
	ft_lstadd(input, ft_lstnew(line, 0));
	g_params->read_lines++;
	if (!fls[0] || !fls[1])
		check_data_sufficiency();
	if (!fls[2])
		error_exit(e_invalid_link);
		
}
