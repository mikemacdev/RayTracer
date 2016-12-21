/* Copyright 2016 Michael MacFarlane
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http ://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

// struct for nodes in list
template <class S>
struct Node
{
    S * object;
    struct Node<S> * next;

    Node()
    {
        object = nullptr;
        next   = nullptr;
    }
};

// class definition
template <class T>
class LinkedList
{
private:

    struct Node<T> * first;
    struct Node<T> * current;

    int size;

public:

    LinkedList();

    const int Size();

    void Prepend(T * obj);
    void Clear();
    void ResetIterator();
    T * Next();
};

// function definitions
template <class T>
LinkedList<T>::LinkedList()
{
    first   = nullptr;
    current = nullptr;
    size = 0;
}

template <class T>
const int LinkedList<T>::Size()
{
    return size;
}

template <class T>
void LinkedList<T>::Prepend(T *obj)
{
    if (obj == nullptr)
        return;

    Node<T> * newNode = new Node<T>;
    newNode->object = obj;
    newNode->next = first;

    first = newNode;
    size++;
}

template <class T>
void LinkedList<T>::Clear()
{
    current = first;
    while (first != nullptr)
    {
        first = current->next;
        current->next = nullptr;
        delete current;
        current = first;
    }
    size = 0;
}

template <class T>
void LinkedList<T>::ResetIterator()
{
    current = first;
}

template <class T>
T * LinkedList<T>::Next()
{
    T * ret = nullptr;

    if (current != nullptr)
    {
        ret = current->object;
        current = current->next;
    }

    return ret;
}
