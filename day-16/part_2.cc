#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

enum class Direction { North, East, South, West };

struct Position
{
	int row;
	int col;
};

struct Ray
{
	Position position;
	Direction direction;
};

std::ostream& operator<<(std::ostream& out, Direction const& direction)
{
	switch (direction)
	{
		case Direction::North:
			out << "North";
			break;
		case Direction::East:
			out << "East";
			break;
		case Direction::South:
			out << "South";
			break;
		case Direction::West:
			out << "West";
			break;
	}

	return out;
}

std::ostream& operator<<(std::ostream& out, Ray const& ray)
{
	out << "{ " << ray.position.row << ", " << ray.position.col << ", " << ray.direction << " }";
	return out;
}

auto initialize_rays(
	Position starting_position,
	Direction starting_direction,
	std::vector<Ray>& rays,
	std::vector<std::vector<bool>>& is_space_energized,
	std::vector<std::vector<char>> const& space)
{
	rays.clear();

	for (auto& line : is_space_energized)
	{
		for (size_t i{0}; i < line.size(); i += 1)
		{
			line[i] = false;
		}
	}

	Position ray_position{starting_position};
	Direction ray_direction{starting_direction};

	switch (space[ray_position.row][ray_position.col])
	{
		case '|':
			switch (ray_direction)
			{
				case Direction::North:
				case Direction::South:
					break;
				case Direction::East:
				case Direction::West:
					ray_direction = Direction::North;
					rays.emplace_back(Ray{ray_position.row, ray_position.col, Direction::South});
					break;
			}
			break;
		case '-':
			switch (ray_direction)
			{
				case Direction::East:
				case Direction::West:
					break;
				case Direction::North:
				case Direction::South:
					ray_direction = Direction::East;
					rays.emplace_back(Ray{ray_position.row, ray_position.col, Direction::West});
					break;
			}
			break;
		case '/':
			switch (ray_direction)
			{
				case Direction::North:
					ray_direction = Direction::East;
					break;
				case Direction::East:
					ray_direction = Direction::North;
					break;
				case Direction::South:
					ray_direction = Direction::West;
					break;
				case Direction::West:
					ray_direction = Direction::South;
					break;
			}
			break;
		case '\\':
			switch (ray_direction)
			{
				case Direction::North:
					ray_direction = Direction::West;
					break;
				case Direction::East:
					ray_direction = Direction::South;
					break;
				case Direction::South:
					ray_direction = Direction::East;
					break;
				case Direction::West:
					ray_direction = Direction::North;
					break;
			}
			break;
	}

	rays.emplace_back(Ray{ray_position, ray_direction});
	is_space_energized[ray_position.row][ray_position.col] = true;
}

auto get_next_position(Ray const& ray, size_t num_rows, size_t num_cols) -> Position
{
	switch (ray.direction)
	{
		case Direction::North:
			if (ray.position.row != 0) { return Position{ray.position.row - 1, ray.position.col}; }
			break;
		case Direction::East:
			if (size_t(ray.position.col) != num_cols - 1) { return Position{ray.position.row, ray.position.col + 1}; }
			break;
		case Direction::South:
			if (size_t(ray.position.row) != num_rows - 1) { return Position{ray.position.row + 1, ray.position.col}; }
			break;
		case Direction::West:
			if (ray.position.col != 0) { return Position{ray.position.row, ray.position.col - 1}; }
			break;
	}

	return Position{-1, -1};
}

auto trace_rays(
	std::vector<Ray>& rays,
	std::vector<std::vector<char>> const& space,
	std::unordered_map<size_t, std::unordered_map<size_t, std::unordered_map<Direction, bool>>>& ray_configurations_seen)
{

	for (size_t i{0}; i < rays.size(); i += 1)
	{
		Ray& ray{rays[i]};

		ray_configurations_seen[ray.position.row][ray.position.col][ray.direction] = true;

		Position next_ray_position{get_next_position(ray, space.size(), space[0].size())};

		if (next_ray_position.row == -1)
		{
			// ray has terminated so remove it from array
			rays[i] = rays.back();;
			rays.pop_back();

			// re-run this index, since the last element now occupies it
			i -= 1;

			continue;
		}

		ray.position = next_ray_position;

		char object_at_new_location{space[ray.position.row][ray.position.col]};

		switch (object_at_new_location)
		{
			case '|':
				switch (ray.direction)
				{
					case Direction::North:
					case Direction::South:
						break;
					case Direction::East:
					case Direction::West:
						ray.direction = Direction::North;

						if (!ray_configurations_seen[ray.position.row][ray.position.col][Direction::South])
						{
							rays.emplace_back(Ray{ray.position, Direction::South});
						}
						break;
				}
				break;
			case '-':
				switch (ray.direction)
				{
					case Direction::East:
					case Direction::West:
						break;
					case Direction::North:
					case Direction::South:
						ray.direction = Direction::East;

						if (!ray_configurations_seen[ray.position.row][ray.position.col][Direction::West])
						{
							rays.emplace_back(Ray{ray.position, Direction::West});
						}
						break;
				}
				break;
			case '/':
				switch (ray.direction)
				{
					case Direction::North:
						ray.direction = Direction::East;
						break;
					case Direction::East:
						ray.direction = Direction::North;
						break;
					case Direction::South:
						ray.direction = Direction::West;
						break;
					case Direction::West:
						ray.direction = Direction::South;
						break;
				}
				break;
			case '\\':
				switch (ray.direction)
				{
					case Direction::North:
						ray.direction = Direction::West;
						break;
					case Direction::East:
						ray.direction = Direction::South;
						break;
					case Direction::South:
						ray.direction = Direction::East;
						break;
					case Direction::West:
						ray.direction = Direction::North;
						break;
				}
				break;
			default:
				break;
		}

		if (ray_configurations_seen[ray.position.row][ray.position.col][ray.direction])
		{
			// ray has been seen so remove it from array
			rays[i] = rays.back();;
			rays.pop_back();

			// re-run this index, since the last element now occupies it
			i -= 1;

			continue;
		}
	}
}

auto get_num_energized(Ray starting_ray, std::vector<std::vector<char>>& space) -> size_t
{
	std::vector<std::vector<bool>> is_space_energized(space.size(), std::vector<bool> (space[0].size(), false));
	std::vector<Ray> rays;

	initialize_rays(starting_ray.position, starting_ray.direction, rays, is_space_energized, space);

	std::unordered_map<size_t, std::unordered_map<size_t, std::unordered_map<Direction, bool>>>
	ray_configurations_seen;

	while(rays.size() != 0)
	{
		trace_rays(rays, space, ray_configurations_seen);

		for (auto const& ray : rays) { is_space_energized[ray.position.row][ray.position.col] = true; }
	}

	size_t num_energized{0};

	for (auto const& line : is_space_energized)
	{
		for (bool const is_energized : line) { if (is_energized) { num_energized += 1; } }
	}

	return num_energized;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: ./part_1 input_file\n";
		return 1;
	}

	std::ifstream input_file(argv[1]);

	std::string line;
	std::getline(input_file, line);

	std::vector<std::vector<char>> space(1);
	std::vector<char>& first_line{space.back()};

	for (const char c : line) { first_line.emplace_back(c); }

	while (std::getline(input_file, line))
	{
		std::vector<char>& space_line{space.emplace_back()};

		for (const char c : line) { space_line.emplace_back(c); }
	}

	size_t max_energized{0};

	// top row
	int row{0};
	for (int col{0}; col < int(space[0].size()); col += 1)
	{
		size_t num_energized{get_num_energized(Ray{Position{row, col}, Direction::South}, space)};

		if (num_energized > max_energized) { max_energized = num_energized; }
	}

	// right side
	int col{int(space[0].size()) - 1};
	for (int row{0}; row < int(space.size()); row += 1)
	{
		size_t num_energized{get_num_energized(Ray{Position{row, col}, Direction::West}, space)};

		if (num_energized > max_energized) { max_energized = num_energized; }
	}

	// bottom row
	row = space[0].size() - 1;
	for (int col{0}; col < int(space[0].size()); col += 1)
	{
		size_t num_energized{get_num_energized(Ray{Position{row, col}, Direction::North}, space)};

		if (num_energized > max_energized) { max_energized = num_energized; }
	}

	// left side
	col = 0;
	for (int row{0}; row < int(space.size()); row += 1)
	{
		size_t num_energized{get_num_energized(Ray{Position{row, col}, Direction::East}, space)};

		if (num_energized > max_energized) { max_energized = num_energized; }
	}

	std::cout << '\n' << "Max Energized: " << max_energized << '\n';

	return 0;
}


