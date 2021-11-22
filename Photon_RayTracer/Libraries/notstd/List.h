#pragma once
#include "UTILITY.h"
/* Information etc
 *
 * This code was Inspired by a tutorial from TheCherno on Youtube.
 * https://www.youtube.com/watch?v=ryRf4Jh_YC0
 *
*/
namespace RinkWilbrink
{
    // We're not allowed to use the std namespace so... here is a notstd namespace.
    namespace notstd
    {
        /* I have heard many complaints from teachers and lectures about templates so here is the reasoning why I use them.
         * 
         * The reason I use templates is because it prevents me from having to duplicate code for each type that 
         * I might need to use this List(Dynamic Array) for, with templates I can set the data type when I instatiate the List,
         * and the compiler will prevent me from inserting other data types into the List,
         * which is commonly called "type safety".
         * 
         * Another advantage to templates is that it can increase run time performance.
         * 
         * I have also read mentions of void*, but I read some negatives about it like the lack of type safety.
         * 
         * Another solution to this problem would be a Double Linked List, this is an ellegant solution, it misses one critical
         * component that I personally feel is really usefull and that is getting the length of the List.
         * This is possible in a Double Linked List but that would involve some weird work arounds.
         *
         * also another interesting thing I read online about templates,
         * is that if there are unused variables inside a template class, they do not get included / allocated, thus reducing memory usage.
         * Although I haven't tested this myself.
         * 
         * A downside I saw mentioned multiple times is that templates let the compiler "write" code for you,
         * which can potentialy add excesive amounts of extra code, which results in larger executable files.
         * 
         * Another downside to templates is that it can increase build times, which can be annoying (C++ is quite slow on its own).
         * 
         * In my opinion; The positives out weight the negatives for me and my use cases.
         * and writing a new List for each data type is not a reasonable solution (code duplication).
         * And the lack of type safety with void* makes it in my opinion a bad solution to this problem.
         * 
         * I feel like I have a solid understanding of what templates do, and I will try to limit my usage but I feel
         * that in this situation it was the best solution.
        */
        template <typename T>
        struct List
        {
        public:
            ~List()
            {
                m_Size = 0;
                delete[] m_Data;
            }

            /// <param name="value">The new value that gets added at the back of the List.</param>
            /// <summary>Add a new instance to the back of the List.</summary>
            void push_back(const T& _value)
            {
                ReAllocate(m_Size + 1);
                m_Data[m_Size - 1] = _value;
            }

            const T& at_first()
            {
                return m_Data[0];
            }

            const T& at_last()
            {
                return m_Data[m_Size - 1];
            }

            /// <summary>RemoveByObject if the List contains the given object.</summary>
            /// <param name="_item">Item to be checked if contained in the List</param>
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

            /// <summary>Return the index of the item given, if it doesn't contain the item, return -1(Just dont check).</summary>
            /// <param name="_item">The item to be checked.</param>
            /// <returns>Index of the Item given, if Item is not stored in the List, return -1.</returns>
            int ItemAtIndex(const T& _item)
            {
                for (size_t i = 0; i < m_Size; i++)
                {
                    if (m_Data[i] == _item)
                    {
                        return i;
                    }
                }
                return -1;
            }

            void RemoveByObject(const T& _pItem)
            {
                for (size_t i = 0; i < m_Size; i++)
                {
                    if (m_Data[i] == _pItem)
                    {
                        remove_at(i);
                        i = m_Size;
                    }
                }
            }

            /// <summary>Return the index of the obj (IDK why you would need this but its in there).</summary>
            /// <param name="_item"></param>
            /// <returns></returns>
            const size_t get_index_from_obj(const T& _item)
            {
                size_t index = 0;

                for (size_t i = 0; i < m_Size; i++)
                {
                    if (m_Data[i] == _item)
                    {
                        index = i;
                        i = m_Size;
                    }
                }

                return index;
            }

            void remove_at(const size_t& index)
            {
                T* newBlocks = new T[m_Size - 1];

                for (size_t i = 0; i < m_Size; i++)
                {
                    if (i > index)
                    {
                        newBlocks[i - 1] = static_cast<T&&>(m_Data[i]);
                    }
                    else if (i < index)
                    {
                        newBlocks[i] = static_cast<T&&>(m_Data[i]);
                    }
                }

                for (size_t i = 0; i < m_Size; i++)
                {
                    m_Data[i].~T();
                }

                delete[] m_Data;
                m_Data = newBlocks;
                m_Size = m_Size - 1;
            }

            /// <summary>Return the size of the List</summary>
            /// <returns>Returns the Size/Length of the Array.</returns>
            size_t get_size()
            {
                return static_cast<size_t>(m_Size);
            }
            
            /// <summary>This is somehow necesarry for pointers to the list.</summary>
            /// <param name="index"></param>
            /// <returns></returns>
            const T& at(const size_t index)
            {
                return m_Data[index];
            }

            const T& operator[](size_t index) const
            {
                return m_Data[index];
            }
        
        private:
            T* m_Data = nullptr;

            size_t m_Size = 0;

            void ReAllocate(const size_t& _newCap)
            {
                T* newBlocks = new T[_newCap];

                for (size_t i = 0; i < m_Size; i++)
                {
                    /* Why should I use std::move?
                     *
                     * From all the online research I did std::move is the best way I could achieve what I wanted,
                     * std::move tells the compiler that the inserted variable is going to get deleted soon and
                     * other variables are allowed to take its data (instead of copying, or referencing it
                     * Online I saw many mention this as "allowing it to get cannibalised").
                     * This makes std::move a superiour way of handeling this problem, unfortunantly I'm not allowed to
                     * use std functions, from my limitted testing static_cast<T&&> does work fine.
                    */
                    //newBlocks[i] = notstd::move(m_Data[i]); // Might have tricked you here huh???
                    newBlocks[i] = static_cast<T&&>(m_Data[i]); // Does the same but notstd::move is a little bit cleaner.
                }

                m_Size = 0;
                
                delete[] m_Data;
                m_Data = newBlocks;
                // Set new Size
                m_Size = _newCap;
            }
        };
    }
}