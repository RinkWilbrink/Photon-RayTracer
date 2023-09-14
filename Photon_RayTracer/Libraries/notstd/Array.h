#pragma once
#include "UTILITY.h"

// We're not allowed to use the std namespace so... here is a notstd namespace.
namespace RinkWilbrink
{ 
    namespace notstd
    {
        /* I have heard many complaints from teachers and lectures about templates so here is the reasoning why I use them.
         *
         * The reasoning for why I use templates can be found in the List.h file
        */
        template <class T>
        struct Array
        {
        public:
            Array(size_t _size)
            {
                T* newBlocks = new T[_size];

                m_Data = newBlocks;
                // Set new Size
                m_Size = _size;
            }
            ~Array()
            {
                m_Size = 0;
                delete[] m_Data;
            }

            size_t get_size()
            {
                return static_cast<size_t>(m_Size);
            }

            /// <summary>RemoveByObject if the List contains the given object.</summary>
            /// <param name="_item">Item to be checked if contained in the Array</param>
            /// <returns>True or False</returns>
            bool Contains(const T& _item)
            {
                for (size_t i = 0; i < m_Size; i++)
                {
                    if (m_Data[i] == _item)
                    {
                        return true;
                    }
                }
                return false;
            }

            /// <summary>Set all items in the array to 0(Empty)</summary>
            void EmptyArray()
            {
                memset(m_Data, 0, m_Size);
            }

            const T& at_first()
            {
                return m_Data[0];
            }

            const T& at_last()
            {
                return m_Data[m_Size - 1];
            }

            T& operator[](size_t index) const
            {
                return m_Data[index];
            }

            /// <summary>ReAllocate the Array with a new size, and set all the previous values to the new Array.</summary>
            void ReAllocate(const size_t& _newCap)
            {
                T* newBlocks = new T[_newCap];

                for (size_t i = 0; i < m_Size; i++)
                {
                    newBlocks[i] = static_cast<T&&>(m_Data[i]); // Does the same but notstd::move is a little bit cleaner.
                }

                m_Size = 0;

                delete[] m_Data;
                m_Data = newBlocks;
                // Set new Size
                m_Size = _newCap;
            }

        private:
            T* m_Data = nullptr;

            size_t m_Size = 0;
        };
    }
}