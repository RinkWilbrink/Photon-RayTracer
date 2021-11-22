#pragma once

namespace RinkWilbrink
{
    // We're not allowed to use the std namespace so... here is a notstd namespace.
    namespace notstd
    {
        /* I have heard many complaints from teachers and lectures about templates so here is the reasoning why I use them.
         *
         * The reasoning for why I use templates can be found in the List.h file
        */
        template <typename T>
        struct Array2D
        {
        public:
            Array2D(const int& _xSize, const int& _ySize)
            {
                T* newBlocks = new T[_xSize * _ySize];
                m_Data = newBlocks;

                // Set new Size
                m_xSize = _xSize;
                m_ySize = _ySize;
            }
            ~Array2D()
            {
                m_xSize = 0;
                m_ySize = 0;
                delete[] m_Data;
            }

            size_t get_size(size_t dimension)
            {
                if (dimension == 0)
                {
                    return static_cast<size_t>(m_xSize);
                }
                else
                {
                    return static_cast<size_t>(m_ySize);
                }
            }

            T& operator()(size_t _x, size_t _y) const
            {
                return m_Data[_x * m_ySize + _y];
            }

            void SetValueAt(const T & _data, size_t _x, size_t _y)
            {
                m_Data[_x * m_ySize + _y] = _data;
            }

            /// <summary>Reallocate the array.</summary>
            /// <param name="_xSize">The width of the Array</param>
            /// <param name="_ySize">the amount of rows in the Array</param>
            void Reallocate(const size_t & _xSize, const size_t & _ySize)
            {
                T* newBlocks = new T[_xSize * _ySize];

                for (size_t x = 0; x < _xSize; x++)
                {
                    for (size_t y = 0; y < _ySize; y++)
                    {
                        //newBlocks[x * m_ySize + y] = notstd::move(m_Data[x * m_ySize + y]);
                        newBlocks[x * m_ySize + y] = static_cast<T&&>(m_Data[x * m_ySize + y]); // Does the same but notstd::move is a little bit cleaner.
                    }
                }

                delete[] m_Data;
                m_xSize = 0;
                m_ySize = 0;
                m_Data = newBlocks;
                // Set new Size
                m_xSize = _xSize;
                m_ySize = _ySize;
            }

        private:
            T* m_Data = nullptr;
            size_t m_xSize = 0;
            size_t m_ySize = 0;
        };
    }
}