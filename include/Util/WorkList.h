//===- WorkList.h -- Internal worklist used in SVF---------------------------//
//
//                     SVF: Static Value-Flow Analysis
//
// Copyright (C) <2013-2017>  <Yulei Sui>
//

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//===----------------------------------------------------------------------===//

/*
 * @file: WorkList.h
 * @author: yesen
 * @date: 06/12/2013
 * @version: 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 */


#ifndef WORKLIST_H_
#define WORKLIST_H_

#include "Util/BasicTypes.h"

#include <assert.h>
#include <cstdlib>
#include <vector>
#include <deque>
#include <set>

namespace SVF
{

/**
 * Worklist with "first come first go" order.
 * New nodes pushed at back and popped from front.
 * Elements in the list are unique as they're recorded by Set.
 */
template<class Data>
class List
{
    class ListNode
    {
    public:
        ListNode(const Data& d)
        {
            data = d;
            next = nullptr;
        }

        ~ListNode() {}

        Data data;
        ListNode* next;
    };

    typedef Set<Data> DataSet;
    typedef ListNode Node;

public:
    List()
    {
        head = nullptr;
        tail = nullptr;
    }

    ~List() {}

    inline bool empty() const
    {
        return (head == nullptr);
    }

    inline bool find(const Data& data) const
    {
        return (nodeSet.find(data) == nodeSet.end() ? false : true);
    }

    void push(const Data& data)
    {
        if (nodeSet.find(data) == nodeSet.end())
        {
            Node* new_node = new Node(data);
            if (head == nullptr)
                head = new_node;// the list is empty
            else
                tail->next = new_node;
            tail = new_node;
        }
    }

    Data pop()
    {
        assert(head != nullptr && "list is empty");
        /// get node from list head
        Node* head_node = head;

        /// change list head to the next node
        head = head->next;
        if (head == nullptr)
            tail = nullptr;	/// the last node is popped.

        Data data = head_node->data;
        nodeSet.erase(data);
        delete head_node;
        return data;
    }

private:
    DataSet nodeSet;
    Node* head;
    Node* tail;
};

/**
 * Worklist with "first in first out" order.
 * New nodes will be pushed at back and popped from front.
 * Elements in the list are unique as they're recorded by Set.
 */
template<class Data>
class FIFOWorkList
{
    typedef std::deque<Data> DataDeque;
public:
    FIFOWorkList() {}

    ~FIFOWorkList() {}

    inline bool empty() const
    {
        return data_list.empty();
    }

    inline bool find(const Data& data) const
    {
        return std::find(data_list.begin(), data_list.end(), data) != data_list.end();
    }

    /**
     * Push a data into the work list.
     */
    inline bool push(const Data& data)
    {
        data_list.push_back(data);
        return true;
    }

    /**
     * Pop a data from the END of work list.
     */
    inline Data pop()
    {
        Data data = front();
        removeFront();
        return data;
    }

    /**
     * Remove a data from the END of work list, no return value
     */
    inline void removeFront()
    {
        assert(!empty() && "work list is empty");
        data_list.pop_front();
    }

    /**
     * Get reference of top data from the END of work list.
     */
    inline Data& front()
    {
        assert(!empty() && "work list is empty");
        Data& data = data_list.front();
        return data;
    }

    /*!
     * Clear all the data
     */
    inline void clear()
    {
        data_list.clear();
    }

private:
    DataDeque data_list;	///< work list using std::vector.
};

/**
 * Worlist with "first in last out" order.
 * New nodes will be pushed at back and popped from back.
 * Elements in the list are unique as they're recorded by Set.
 */
template<class Data>
class FILOWorkList
{
    typedef std::vector<Data> DataVector;
public:
    FILOWorkList() {}

    ~FILOWorkList() {}

    inline bool empty() const
    {
        return data_list.empty();
    }

    inline bool find(const Data& data) const
    {
        return std::find(data_list.begin(), data_list.end(), data) != data_list.end();
    }

    /**
     * Push a data into the work list.
     */
    inline bool push(const Data& data)
    {
        data_list.push_back(data);
        return true;
    }

    /**
     * Pop a data from the END of work list.
     */
    inline Data pop()
    {
        Data data = back();
        removeBack();
        return data;
    }

    /**
     * Remove a data from the END of work list, no return value
     */
    inline void removeBack()
    {
        assert(!empty() && "work list is empty");
        data_list.pop_back();
    }

    /**
     * Get reference of top data from the END of work list.
     */
    inline Data& back()
    {
        assert(!empty() && "work list is empty");
        Data& data = data_list.back();
        return data;
    }

    /*!
     * Clear all the data
     */
    inline void clear()
    {
        data_list.clear();
    }

private:
    DataVector data_list;	///< work list using std::vector.
};

} // End namespace SVF

#endif /* WORKLIST_H_ */
