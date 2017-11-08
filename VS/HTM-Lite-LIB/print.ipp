// C++ port of Nupic HTM with the aim of being lite and fast
//
// Copyright (c) 2017 Henk-Jan Lebbink
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero Public License version 3 as
// published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Affero Public License for more details.
//
// You should have received a copy of the GNU Affero Public License
// along with this program.  If not, see http://www.gnu.org/licenses.

#pragma once
#include <string>
#include <array>
#include <vector>
#include <iomanip>      // std::setw
#include <sstream>      // std::stringstream

#include "constants.ipp"
#include "tools.ipp"
#include "types.ipp"

namespace htm
{
	namespace print
	{
		using namespace htm::types;

		template <int SIZE>
		void print_bitset(
			const Bitset_Tiny<SIZE>& bitset)
		{
			std::ostringstream result;
			result << "(count = " << bitset.count() << "):";
			for (auto i = 0; i < SIZE; ++i) if (bitset.get(i)) result << " " << i;
			result << "\n";
			return result.str();
		}

		template <int SIZE>
		std::string print_bitset(
			const Bitset<SIZE>& bitset)
		{
			std::ostringstream result;
			result << "(count = " << bitset.count() << "):";
			for (auto i = 0; i < SIZE; ++i) if (bitset[i]) result << " " << i;
			result << "\n";
			return result.str();
		}

		template <int SIZE>
		std::string print_bitset(
			const Bitset_Sparse<SIZE>& bitset)
		{
			std::ostringstream result;
			result << "(count = " << bitset._data.size() << "):";
			for (auto i = 0; i < bitset._data.size(); ++i) if (bitset._data[i]) result << " " << i;
			result << "\n";
			return result.str();
		}

		template <int SIZE1, int SIZE2>
		std::string print_bitset(
			const Bitset2<SIZE1, SIZE2>& bitset)
		{
			std::ostringstream result;
			result << "(count = " << tools::count(bitset) << "):";
			for (auto i1 = 0; i1 < SIZE1; ++i1)
			{
				for (auto i2 = 0; i2 < SIZE2; ++i2)
				{
					if (bitset[i1][i2]) result << " " << i1 << "~" << i2;
				}
			}
			result << "\n";
			return result.str();
		}

		template <typename P>
		std::string print_active_cells(
			const Bitset_Sparse<P::N_CELLS>& cells)
		{
			Bitset<P::N_COLUMNS> columns_local;
			columns_local.reset();
			std::ostringstream result;

			int n_active_cells = 0;

			for (auto global_cell_id = 0; global_cell_id < P::N_CELLS; ++global_cell_id)
			{
				if (cells.get(global_cell_id))
				{
					const auto tup = tools::global_2_local_cell<P::N_COLUMNS, P::N_BITS_CELL>(global_cell_id);
					const auto column_i = std::get<1>(tup);
					columns_local[column_i] = true;
					n_active_cells++;
				}
			}
			const auto n_columns = columns_local.count();

			result << " [n_columns = " << n_columns << ", n_cells = " << n_active_cells << "]:" << std::endl;

			if (n_columns > 0)
			{
				for (auto column_i = 0; column_i < P::N_COLUMNS; ++column_i)
				{
					if (columns_local[column_i]) result << std::setw(5) << column_i;
				}
				result << "\n";
				for (auto cell_i = 0; cell_i < P::N_CELLS_PC; ++cell_i)
				{
					for (auto column_i = 0; column_i < P::N_COLUMNS; ++column_i)
					{
						if (columns_local[column_i])
						{
							const auto global_cell_id = tools::local_2_global_cell<P::N_BITS_CELL>(cell_i, column_i);

							result << ((cells.get(global_cell_id)) ? "    X" : "    .");
						}
					}
					result << "\n";
				}
			}
			return result.str();
		}

		template <typename P>
		std::string print_active_cells(
			const Bitset_Compact<P::N_CELLS>& cells)
		{
			Bitset<P::N_COLUMNS> columns_local;
			columns_local.reset();
			std::ostringstream result;

			int n_active_cells = 0;

			for (auto global_cell_id = 0; global_cell_id < P::N_CELLS; ++global_cell_id)
			{
				if (cells.get(global_cell_id))
				{
					const auto tup = tools::global_2_local_cell<P::N_COLUMNS, P::N_BITS_CELL>(global_cell_id);
					const auto column_i = std::get<1>(tup);
					columns_local[column_i] = true;
					n_active_cells++;
				}
			}
			const auto n_columns = columns_local.count();

			result << " [n_columns = " << n_columns << ", n_cells = " << n_active_cells << "]:" << std::endl;

			if (n_columns > 0)
			{
				for (auto column_i = 0; column_i < P::N_COLUMNS; ++column_i)
				{
					if (columns_local[column_i]) result << std::setw(5) << column_i;
				}
				result << "\n";
				for (auto cell_i = 0; cell_i < P::N_CELLS_PC; ++cell_i)
				{
					for (auto column_i = 0; column_i < P::N_COLUMNS; ++column_i)
					{
						if (columns_local[column_i])
						{
							const auto global_cell_id = tools::local_2_global_cell<P::N_BITS_CELL>(cell_i, column_i);

							result << ((cells.get(global_cell_id)) ? "    X" : "    .");
						}
					}
					result << "\n";
				}
			}
			return result.str();
		}

		template <int N_COLUMNS>
		std::string print_active_columns(
			const Bitset<N_COLUMNS>& column_active,
			const int width)
		{
			int sum = 0;
			int counter = 0;
			std::ostringstream result;

			while (counter < N_COLUMNS)
			{
				for (auto i = 0; i < width; ++i)
				{
					if (counter < N_COLUMNS)
					{
						if (column_active[counter])
						{
							result << "X";
							sum++;
						}
						else
						{
							result << ".";
						}
					}
					counter++;
				}
				result << "\n";
			}
			result << "number active columns " << sum << ".";
			return result.str();
		}

		template <int N_SENSORS>
		std::string print_sensor_activity(
			const Bitset_Compact<N_SENSORS>& sensor_activity,
			const int width)
		{
			int sum = 0;
			int counter = 0;
			std::ostringstream result;

			while (counter < N_SENSORS)
			{
				for (auto i = 0; i < width; ++i)
				{
					if (counter < N_SENSORS)
					{
						if (sensor_activity.get(counter))
						{
							result << "X";
							sum++;
						}
						else
						{
							result << ".";
						}
					}
					counter++;
				}
				result << "\n";
			}
			result << "number of active sensors: " << sum << std::endl;
			return result.str();
		}

		template <typename P>
		std::string print_sensor_activity2(
			const Bitset_Compact<P::N_SENSORS>& sensor_activity1,
			const Bitset_Compact<P::N_SENSORS>& sensor_activity2,
			const int width)
		{
			std::ostringstream result;

			auto counter1 = 0;
			auto counter2 = 0;
			auto counter3 = 0;

			while (counter1 < P::N_SENSORS)
			{
				for (auto i = 0; i < width; ++i)
				{
					result << ((counter1 < P::N_SENSORS) ? ((sensor_activity1.get(counter1)) ? "X" : ".") : "-");
					counter1++;
				}
				result << " | ";
				for (auto i = 0; i < width; ++i)
				{
					result << ((counter2 < P::N_SENSORS) ? ((sensor_activity2.get(counter2)) ? "X" : ".") : "-");
					counter2++;
				}
				result << " | ";

				for (auto i = 0; i < width; ++i)
				{
					result << ((counter3 < P::N_SENSORS) ? ((sensor_activity1.get(counter3) == sensor_activity2.get(counter3)) ? "." : "X") : "-");
					counter3++;
				}
				result << "\n";
			}
			return result.str();
		}

		template <typename P>
		std::string print_boost_factors(
			const Layer<P>& layer,
			const int width)
		{
			std::ostringstream result;

			int column_i = 0;
			while (column_i < P::N_COLUMNS)
			{
				const auto& column = layer[column_i];
				for (int i1 = 0; i1 < width; ++i1)
				{
					if (column_i < P::N_COLUMNS)
					{
						result << " " << std::fixed << std::setprecision(3) << column.boost_factor;
					}
					column_i++;
				}
				result << "\n";
			}
			return result.str();
		}

		template <typename P>
		std::string print_dd_synapses(
			const Column<P>& column)
		{
			std::ostringstream result;
			const auto n_segments = column.dd_segment_count;

			for (auto segment_i = 0; segment_i < n_segments; ++segment_i)
			{
				const auto n_synapses = column.dd_synapse_count[segment_i];
				const auto& dd_synapse_permanence_segment = column.dd_synapse_permanence[segment_i];
				const auto& dd_synapse_delay_origin_segment = column.dd_synapse_delay_origin[segment_i];

				result << "Column " << std::setw(4) << column.id << ": segment " << segment_i << "/ " << n_segments << ": permanence:";
				for (auto synapse_i = 0; synapse_i < n_synapses; ++synapse_i)
				{
					result << " " << std::fixed << std::setprecision(3) << dd_synapse_permanence_segment[synapse_i];
				}
				result << "\n";

				if (true)
				{
					result << "Column " << std::setw(4) << column.id << ": segment " << segment_i << "/ " << n_segments << ": cell org  :";
					for (auto synapse_i = 0; synapse_i < n_synapses; ++synapse_i)
					{
						const auto delay_and_cell_id = dd_synapse_delay_origin_segment[synapse_i];
						result << " " << std::setw(5) << htm::tools::get_global_cell_id(delay_and_cell_id);
					}
					result << "\n";
				}
			}
			return result.str();
		}
		
		template <typename P>
		std::string print_dd_synapses(
			const Layer<P>& layer)
		{
			std::ostringstream result;
			for (auto column_i = 0; column_i < P::N_COLUMNS; ++column_i)
			{
				const auto& column = layer[column_i];
				if (column.dd_segment_count > 0) result << print_dd_synapses(column);
			}
			return result.str();
		}
		
		template <typename P>
		std::string print_pd_synapses(
			const Column<P>& column)
		{
			std::ostringstream result;
			//const auto n_segments = column.dd_segment_count;
			result << "Column " << std::setw(4) << column.id << ": permanence:";

			for (auto synapse_i = 0; synapse_i < P::SP_N_PD_SYNAPSES; ++synapse_i)
			{
				const float permanence = column.pd_synapse_permanence[synapse_i];
				result << " " << std::fixed << std::setprecision(3) << permanence;
			}
			result << "\n";

			if (false)
			{
				result << "Column " << std::setw(4) << column.id << ": cell org  :";
				for (auto synapse_i = 0; synapse_i < P::SP_N_PD_SYNAPSES; ++synapse_i)
				{
					const auto sensor_idx = column.pd_synapse_origin[synapse_i];
					result << " " << std::setw(5) << sensor_idx;
				}
				result << "\n";
			}
			return result.str();
		}

		template <typename P>
		std::string print_pd_synapses(
			const Layer<P>& layer)
		{
			std::ostringstream result;
			for (auto column_i = 0; column_i < P::N_COLUMNS; ++column_i)
			{
				result << print_pd_synapses(layer[column_i]);
			}
			return result.str();
		}

		std::string print_int_array(const std::vector<int>& a, const int size) {
			std::ostringstream result;
			for (auto i = 0; i < size; ++i) result << " " << a[i];
			result << "\n";
			return result.str();
		}

		std::string print_float_array(const std::vector<float>& a, const int size)
		{
			std::ostringstream result;
			result << std::fixed << std::setprecision(3);
			for (auto i = 0; i < size; ++i) result << " " << a[i];
			result << "\n";
			return result.str();
		}
	}
}