/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 13:44:12 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/04/26 14:43:38 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
# include <stdint.h>

int main(void)
{

    uint32_t color = 0xAAFFFFFF;
    uint32_t mask = 0x00FFFFFF;
    uint32_t red = (color >> 24) & 0xFF;
    red = red ^ 0xFF;

    printf("original color: %x\n", color);

    color = color & mask;
    
    printf("intermediate color: %x\n", color);
    printf("new red: %x\n", red);
    printf("final color: %x\n", color | red << 24);

}