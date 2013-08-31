#define FOREACH( x, y ) { \
      int size = x.Size(); \
      x.Reset(); \
      for (int i = 0; i < size; i++){ \
         y \
         x++; \
      } \
   }

#define PFOREACH( x, y ) {\
   int size = x->Size(); \
   x->Reset(); \
   for (int i = 0; i < size; i++){ \
         y \
         (*x)++; \
      } \
   }

template <class S>
struct TNode{
   TNode() {next = NULL;}   //constructor

   //data members
   S value;                //data stored in a TNode
   LPCOLESTR name;         //optional name of the TNode
   TNode<S>* next;          //pointer to next TNode
};

template <class T>
class TList{

protected:
   //Data members
   TNode<T> *first;
   TNode<T> *current;
   TNode<T> *last;
   int size;
   
public:
   //default constructor
   TList() : first(NULL), current(NULL), last(NULL), size(0) {;}
   //copy constructor
   TList(const TList<T>& s)
   {
      TNode<T> *lptr = s.first;
      TNode<T> *nptr;
      
      //is the list we're copying empty
      if (lptr == NULL)
      {
         current = first = last = NULL;
         size = 0;
      }
      else
      {
         //create the first TNode
         nptr = new TNode<T>;
         nptr->value = lptr->value;
         nptr->name = lptr->name;
         
         //check if the TNode we're copying is the current TNode of the other list
         if (lptr == s.current)
            current = nptr;
         
         //set the first pointer
         first = nptr;
         
         //move on to the next TNode to copy
         lptr = lptr->next;
         
         while(lptr)
         {
            //create the next TNode
            nptr->next = new TNode<T>;
            nptr = nptr->next;
            nptr->value = lptr->value;
            nptr->name = lptr->name;
            
            //check if the TNode we're copying is the current TNode of the other list
            if (lptr == s.current)
               current= nptr;
            
            //move on to the next TNode to copy
            lptr = lptr->next;
         }
         
         //finish the last TNode
         nptr->next = NULL;
         
         //set the last pointer
         last = nptr;
         
         //set the size of the new list
         size = s.size;
      }
   }
   
   //Destructor
   ~TList()
   {
      //Delete all the TNodes in the list.
      TNode<T>* nptr = first;
      current = first;
      
      while(current)
      {
         current = current->next;
         delete nptr;
         nptr = current;
      }
      
      first = current = last = NULL;
      size = 0;
   }
   
   //Insertion functions
   void InsertBefore(T e, LPCOLESTR theName = NULL)
   {
      //Construct a new TNode
      TNode<T> *nptr = new TNode<T>;
      
      if (size == 0)
      {
         nptr->value = e;
         nptr->name = theName;
         nptr->next = NULL;
         current = first = last = nptr;
      }
      else
      {
         //To avoid traversals, insert after current and then swap values
         nptr->value = current->value;
         nptr->next = current->next;
         nptr->name = current->name;
         current->value = e;
         current->next = nptr;
         current->name = theName;
         if (current == last)
            last = nptr;
      }
      size++;
   }
   
   void InsertAfter(T e, LPCOLESTR theName = NULL)
   {
      //Construct a new TNode and fill it with the appropriate value
      TNode<T> *nptr = new TNode<T>;
      nptr->value = e;
      nptr->name = theName;
      
      if (size == 0)
      {
         nptr->next = NULL;
         current = first = last = nptr;
      }
      else
      {
         nptr->next = current->next;
         current->next = nptr;
         if (current == last)
            last = nptr;
         current = nptr;
      }
      size++;
   }
   
   void Remove()
   {
      TNode<T> *nptr;
      if (size == 0)
      {
         //OutputDebugString("Attempted to remove from empty list.\n");
         return;
      }
      else if (size == 1)
      {
         delete current;
         first = current = last = NULL;
      }
      else if (current == last)
      {
         nptr = first;
         while (nptr->next != current)
            nptr = nptr->next;
         
         nptr->next = NULL;
         delete current;
         
         current = last = nptr;
      }
      else
      {
         nptr = current->next;
         current->value = nptr->value;
         current->name = nptr->name;
         current->next = nptr->next;
         delete nptr;
      }
      size--;
   }
   
   TList<T>& operator++(int)
   {
      if (current != last)
         current = current->next;
      return *this;
   }
   
   TList<T>& operator--(int)
   {
      if (current != first)
      {
         TNode<T>* nptr = first;
         while (nptr->next != current)
            nptr = nptr->next;
         current = nptr;
      }
      return *this;
   }
   
   void GotoLast()
   {
      current = last;
   }

   T Retrieve() const
   {
      return current->value;
   }
   
   void Update(T e)
   {
      if (size != 0)
         current->value = e;
   }
   
   int Includes(T e)
   {
      TNode<T> *nptr = first;
      while (nptr && (nptr->value != e))
         nptr = nptr->next;
      if (nptr)
         current = nptr;
      return (nptr != NULL);
   }
   
   int FindByName( LPCOLESTR theName )
   {
      TNode<T> *nptr = first;

      //This nasty chain of whiles and ifs is necessary because not all nodes
      //may actually be named.
      while (nptr){
         if (nptr->name){
            if (wcscmp( nptr->name, theName ) == 0)
               break;
         }
         nptr = nptr->next;
      }

      if (nptr != NULL){
         current = nptr;
         return true;
      }
      else
         return false;
   }

   LPCOLESTR GetName()
   {
      if (size != 0)
         return current->name;
      else
         return NULL;
   }

   void Reset()
   {
      current = first;
   }

   int Size()
   {
      return size;
   }
};