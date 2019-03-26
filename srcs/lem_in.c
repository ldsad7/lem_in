/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsprigga <bsprigga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 17:40:41 by bsprigga          #+#    #+#             */
/*   Updated: 2019/03/26 18:18:50 by bsprigga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lem_in.h"

/*
void	check_coordinates(void)
{
 	t_room		*start_of_list;
 	t_room		*tmp;

 	if (!g_params)
 		return ;
 	start_of_list = g_params->start_of_list;
 	while (start_of_list)
 	{
 		tmp = start_of_list->next;
 		while (tmp)
 		{
 			if (tmp->coord_x == start_of_list->coord_x &&
 				tmp->coord_y == start_of_list->coord_y)
 				error_exit();
 			tmp = tmp->next;
 		}
		start_of_list = start_of_list->next;
	}
}
*/

int		num_of_nghbrs(t_neighbour *neighbour)
{
	int		i;

	i = 0;
	while (neighbour)
	{
		neighbour = neighbour->next;
		i++;
	}
	return (i);
}

int		sum_of_paths(t_path **paths, int flows)
{
	int		sum;
	int		i;

	i = 0;
	sum = 0;
	while (i < flows)
	{
		sum += paths[i]->len_seq;
		i++;
	}
	return (sum);
}

int		ft_min(int a, int b, int c)
{
	if (a <= b && a <= c)
		return (a);
	if (b <= a && b <= c)
		return (b);
	return (c);
}

t_room	**lst_to_array(t_neighbour *nghbr, int len)
{
	int		i;
	t_room	**res;

	res = (t_room **)malloc(sizeof(*res) * len);
	i = 0;
	while (nghbr)
	{
		res[i] = nghbr->room;
		nghbr = nghbr->next;
		i++;
	}
	return (res);
}

int	new_ants_move_in_path(int i, t_room *room, int *space_flag,
								int **nr_ants_to_move_in_paths)
{
	int		fl;

	fl = 0;
	if ((*nr_ants_to_move_in_paths)[i] > 0 && g_params->nr_ants > 0)
	{
		room->ant_nr = g_params->nr_ants;
		g_params->nr_ants--;
		if (*space_flag && room != g_params->start)
			fl += write(1, " ", 1);
		else if (!(*space_flag))
			*space_flag = 1;
		if (room != g_params->start)
			fl += ft_printf("L%i-%s", room->ant_nr, room->name);
		(*nr_ants_to_move_in_paths)[i]--;
	}
	return (fl);
}

int	print_existing_ants_movement(t_room **room, int *space_flag)
{
	int		fl;

	fl = 0;
	if ((*room)->next_elem == g_params->end && (*room)->ant_nr)
	{
		if (*space_flag)
			fl += write(1, " ", 1);
		else
			*space_flag = 1;
		fl += ft_printf("L%i-%s", (*room)->ant_nr, (*room)->next_elem->name);
		(*room)->ant_nr = 0;
	}
	while ((*room)->prev_path != g_params->start && (*room)->prev_path && (*room)->prev_path->ant_nr)
	{
		(*room)->ant_nr = (*room)->prev_path->ant_nr;
		(*room)->prev_path->ant_nr = 0;
		if (*space_flag)
			fl += write(1, " ", 1);
		else
			*space_flag = 1;
		fl += ft_printf("L%i-%s", (*room)->ant_nr, (*room)->name);
		(*room) = (*room)->prev_path;
	}
	return (fl);
}

void	iter_ants_move(int nr_steps, t_room **room_arr,
						int *nr_ants_to_move_in_paths, int cnt_paths)
{
	t_room		*tmp_room;
	int			i;
	int			fl;
	int			space_flag;

	nr_steps += 1;
	while (nr_steps--)
	{
		space_flag = 0;
		i = 0;
		fl = 0;
		while (i < cnt_paths)
		{
			tmp_room = room_arr[i];
			if (tmp_room == g_params->start)
				tmp_room->ant_nr = g_params->nr_ants;
			fl += print_existing_ants_movement(&tmp_room, &space_flag);
			fl += new_ants_move_in_path(i, tmp_room, &space_flag, &nr_ants_to_move_in_paths);
			if (room_arr[i]->next_elem != g_params->end)
				room_arr[i] = room_arr[i]->next_elem;
			i++;
		}
		if (fl)
			write(1, "\n", 1);
	}
}

void	rooms_arr_setup(t_room ***room_arr, int **nr_ants_to_move_in_paths,
						int cnt_paths, int sum_paths)
{
	t_path		*tmp;
	int			i;

	if (!(*room_arr = (t_room **)malloc(sizeof(t_room *) * cnt_paths)))
		exit(0);
	if (!(*nr_ants_to_move_in_paths = (int *)malloc(sizeof(int) * cnt_paths)))
		exit(0);
	i = 0;
	tmp = g_params->start_of_list_of_paths;
	while (tmp)
	{
		(*nr_ants_to_move_in_paths)[i] = (g_params->nr_ants + sum_paths)
									/ cnt_paths - tmp->len_seq + ((g_params->nr_ants + sum_paths) % cnt_paths != 0);
		if (!(tmp->seq))
			(*room_arr)[i] = g_params->start;
		else
			(*room_arr)[i] = tmp->seq->room;
		tmp = tmp->next;
		++i;
	}
}

void	print_paths(int nr_steps)
{
	t_path		*tmp;
	int			sum_paths;
	int			cnt_paths;
	t_room		**room_arr;
	int			*nr_ants_to_move_in_paths;

	sum_paths = 0;
	cnt_paths = 0;
	tmp = g_params->start_of_list_of_paths;
	while (tmp)
	{
		sum_paths += tmp->len_seq;
		cnt_paths++;
		tmp = tmp->next;
	}
	rooms_arr_setup(&room_arr, &nr_ants_to_move_in_paths, cnt_paths, sum_paths);
	iter_ants_move(nr_steps, room_arr, nr_ants_to_move_in_paths, cnt_paths);
	free(nr_ants_to_move_in_paths);
	free(room_arr);
}

void	correct_paths(void)
{
	t_neighbour		*seq;
	t_path			*paths;

	paths = g_params->start_of_list_of_paths;
	while (paths)
	{
		if (!(seq = paths->seq))
		{
			g_params->start->next_elem = g_params->end;
			paths = paths->next;
			continue ;
		}
		else
			seq->room->prev_path = g_params->start;
		while (seq)
		{
			if (seq->next)
			{
				seq->room->next_elem = seq->next->room;
				seq->next->room->prev_path = seq->room;
			}
			else
				seq->room->next_elem = g_params->end;
			seq = seq->next;
		}
		paths = paths->next;
	}
	return ;
}

void	print_paths_double(void)
{
	int				i;
	t_neighbour		*seq;
	t_path			*paths;

	paths = g_params->start_of_list_of_paths;
	i = 1;
	while (paths)
	{
		// printf("%d\n", paths->len_seq);
		// printf("path №%d\n", i);
		
		//seq = paths->seq;
		if (!(seq = paths->seq))
		{
			g_params->start->next_elem = g_params->end; //!!!
			paths = paths->next;
			printf("start\n");
			printf("end\n");
			continue ;
		}
		else
			seq->room->prev_path = g_params->start;
		printf("start\n");
		seq->room->prev_path = g_params->start;
		while (seq)
		{
			printf("%s\n", seq->room->name);
			if (seq->next)
			{
				seq->room->next_elem = seq->next->room;
				seq->next->room->prev_path = seq->room;
			}
			else
				seq->room->next_elem = g_params->end;
			seq = seq->next;
		}
		printf("end\n");
		// printf("-----------------\n");
		paths = paths->next;
		i++;
	}
	// printf("-----------------\n");
	return ;
}

int		main(int argc, char **argv)
{
	t_path		*paths;
	int			flows;
	int			nr_steps;
	int			fd;
	char		*line;

	argc = 0;
	argv = NULL;
	if ((fd = open("tmp.txt", O_WRONLY|O_CREAT, 0777)) < 0)
		exit(0);
	read_input(fd);
	// check_coordinates();
	flows = ft_min(g_params->nr_ants,
			num_of_nghbrs(g_params->start->neighbours),
			num_of_nghbrs(g_params->end->neighbours));
	nr_steps = algorithm(flows, &paths);
	// printf("%d\n", nr_steps);
	correct_paths();
	// print_paths_double();
	line = NULL;
	close(fd);
	if ((fd = open("tmp.txt", O_RDONLY)) < 0)
		exit(0);
	while (get_next_line(fd, &line) > 0)
	{
		ft_printf("%s\n", line);
		free(line);
	}
	ft_printf("\n");
	close(fd);
	print_paths(nr_steps);
	free_g_params();
	return (0);
}
