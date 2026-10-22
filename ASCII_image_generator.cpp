#define STB_IMAGE_IMPLEMENTATION //нужно для корректной работы stb
#include "stb_image.h" 

#include <iostream>
#include <vector>

int main()
{
	int image_height_px, image_width_px;
	int image_color_channels; //параметры входного изображения
	
	unsigned char* image = stbi_load("input.jpg", &image_width_px, &image_height_px, &image_color_channels, 3); //загрузка изображения
	
	if (!image)
	{
		std::cerr << "Изображение не загрузилось. Проверьте формат/название/расположение файла." << std::endl;
		return 1;
	}

	std::cout << "Размер изображения:  " << image_width_px << "x" << image_height_px << " пикселей;" << std::endl; // для отладки

	const int ASCII_output_width_symbols = 80;
	const int ASCII_output_height_symbols = 40; //Сетка для отладки. Поскольку для отрисовки используются заглавные символы, то есть,
						    //высота символа примерно в два раза выше его широты, для стартовых значений сетки заданы обратные значения 

	std::vector<int> image_luminance(ASCII_output_width_symbols * ASCII_output_height_symbols);

	for (int ASCII_output_y_coordinate = 0; ASCII_output_y_coordinate < ASCII_output_height_symbols; ASCII_output_y_coordinate++) //составление карты яркостей по сетке вывода
	{
		for (int ASCII_output_x_coordinate = 0; ASCII_output_x_coordinate < ASCII_output_width_symbols; ASCII_output_x_coordinate++)
		{
			int image_x_coordinate = ASCII_output_x_coordinate * image_width_px / ASCII_output_width_symbols;
			int image_y_coordinate = ASCII_output_y_coordinate * image_height_px / ASCII_output_height_symbols;

			int image_byte_index = 3 * (image_y_coordinate * image_width_px + image_x_coordinate);

			int pixel_luminance = (image[image_byte_index] * 299 + image[image_byte_index + 1] * 587 + image[image_byte_index + 2] * 114) / 1000;
			image_luminance[ASCII_output_y_coordinate * ASCII_output_width_symbols + ASCII_output_x_coordinate] = pixel_luminance;
		}
	}
	
	//std::cout << image_luminance[0] << std::endl;
	//
	const std::string symbol_alphabet = " .:-=+*#%@"; //Символы из которых отрисовывается изображение 
	const int symbol_alphabet_size = (int)symbol_alphabet.size() - 1;
	
	for (int ASCII_output_y_coordinate = 0; ASCII_output_y_coordinate < ASCII_output_height_symbols; ASCII_output_y_coordinate++) //сопоставление символов по карте яркостей и вывод на консоль
	{
		for (int ASCII_output_x_coordinate = 0; ASCII_output_x_coordinate < ASCII_output_width_symbols; ASCII_output_x_coordinate++)
		{
			int pixel_luminance = image_luminance[ASCII_output_y_coordinate * ASCII_output_width_symbols + ASCII_output_x_coordinate];
			int ASCII_output_index = pixel_luminance * symbol_alphabet_size / 255;
			
			std::cout << symbol_alphabet[ASCII_output_index];
		}

		std::cout << std::endl;
	}

	stbi_image_free(image);
	return 0;
}

