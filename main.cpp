#include <cstddef>
#include <iostream>
#include <sstream>
#include <random>
#include <chrono>

typedef char _TCHAR;

struct tore
{
  int id;
  bool win;
  tore* next;
};

bool calc_win(int z_auto, int z_moderator, int z_spieler, int z_wechsel);

int main(int argc, _TCHAR* argv[])
{
  // zufallsvars
  int z_auto = 0;  // 0<=z_auto<=3
  int z_moderator; // 0<=z_moderator<=2
  int runs = 0;    // anzahl der durchläufe
  int z_spieler = 0; // 0<=z_auto<=3
  int z_wechsel = 0; // 0<=z_wechsel<=2

  int count_spieler_wechsel_won = 0;
  int count_spieler_nowechsel_won = 0;
  int count_spieler_nowechsel_lose = 0;
  int count_spieler_wechsel_lose = 0;


  std::basic_istringstream<_TCHAR> ss_arg1(argv[1]);
  ss_arg1 >> runs;

  printf("runs: %d, argc: %d\n",runs,argc );

  // random numer generator:
  std::mt19937_64 rng;
  // initialize the random number generator with time-dependent seed
  uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
  rng.seed(ss);    
  // initialize a uniform distribution between 0 and 1
  std::uniform_real_distribution<double> unif(0, 1);
  // ready to generate random numbers
  
  for (int i = 0; i < runs; i++)
  {
      z_auto      = (int)3.0*unif(rng);
      z_moderator = (int)2.0*unif(rng);
      z_spieler   = (int)3.0*unif(rng);
      z_wechsel   = (int)2.0*unif(rng);
      bool win = calc_win(z_auto,z_moderator,z_spieler,z_wechsel);
      if(win && (z_wechsel == 0))
      {
        count_spieler_nowechsel_won++;
      }
      if(win && (z_wechsel == 1))
      {
        count_spieler_wechsel_won++;
      }
      if(!win && (z_wechsel == 0))
      {
        count_spieler_nowechsel_lose++;
      }
      if(!win && (z_wechsel == 1))
      {
        count_spieler_wechsel_lose++;
      }
      //printf("\n");

  }
  printf("runs: %d, win_wechsel: %d, win_nowechsel: %d,lose_wechsel: %d, lose_nowechsel: %d",runs,count_spieler_wechsel_won,count_spieler_nowechsel_won,count_spieler_wechsel_lose, count_spieler_nowechsel_lose);
  return 0;
}    

bool calc_win(int z_auto, int z_moderator, int z_spieler, int z_wechsel)
{
  tore* tor_liste;
  tore* p_spieler = nullptr;
  tor_liste = nullptr;
  int end = 3;
  tore *neu;
  for(int i=0;i<end;i++)
  {  
    if(!tor_liste)
    {  
      tor_liste = new tore;
      neu = tor_liste;
      neu-> next = nullptr;
    }
    else
    {
      neu->next = new tore;
      neu = neu->next;
      neu->next =nullptr;
    }
    neu->id = i;
    if(i == z_auto)
      neu->win = true;
    else
      neu->win = false;
    if (i == z_spieler)
    {
      p_spieler = neu;
    }
  }
/*    
  tore* helper_print = tor_liste;
  while(helper_print != nullptr)
  {  
    printf("(%d) %d ",helper_print->id, helper_print->win);
    helper_print = helper_print ->next;
  }
  printf("spieler_id: %d ", p_spieler->id);
  printf("\n");
*/
  // remove ziege
  tore* p_remove = nullptr;
  tore* helper_remove = tor_liste;
  tore* helper_last = tor_liste;
  bool removed = false;
  if (p_spieler->win == true) // moderator has a choice
  {
    if (z_moderator) //take first ziege
    {
      while((helper_remove !=nullptr)&& (!removed))
      {
        if(helper_remove->win == false)
        {
          if(helper_remove == tor_liste)
          {
            tor_liste = tor_liste->next;
            delete helper_remove;
          }
          else
          {
            helper_last->next = helper_remove->next;
            delete helper_remove;
          }
          removed = true;
        }
        else // look further for a ziege
        {
          helper_last = helper_remove;
          helper_remove = helper_remove->next;
        }
      }
    }
    else //take second ziege
    {
      bool second = false;
      while((helper_remove !=nullptr)&& (!removed))
      {
        if((helper_remove->win == false) && second)
        {
          if(helper_remove == tor_liste)
          {
            tor_liste = tor_liste->next;
            delete helper_remove;
          }
          else
          {
            helper_last->next = helper_remove->next;
            delete helper_remove;
          }
          removed = true;
        }
        else // look further for a ziege 
        {
          if(helper_remove->win == false)
            second = true;
          helper_last = helper_remove;
          helper_remove = helper_remove->next;
        }
      }
    }
  }    
  else // moderator has no choice there is only one ziege he can take
  {
    while((helper_remove !=nullptr)&& (!removed))
    {
      if((helper_remove->win == false) && (p_spieler != helper_remove))
      {
        if(helper_remove == tor_liste)
        {
          tor_liste = tor_liste->next;
          delete helper_remove;
        }
        else
        {
          helper_last->next = helper_remove->next;
          delete helper_remove;
        }
        removed = true;
      }
      else // look further for a ziege
      {
        helper_last = helper_remove;
        helper_remove = helper_remove->next;
      }
    }
  }

/*  
  helper_print = tor_liste;
  while(helper_print != nullptr)
  {  
    printf("(%d) %d ",helper_print->id, helper_print->win);
    helper_print = helper_print ->next;
  }
  printf("spieler_id: %d ", p_spieler->id);
  printf("\n");
*/
  bool return_win = 0;
  if(z_wechsel == 0)
    return_win = p_spieler->win;
  else 
    return_win = (p_spieler->win == false);

  tore* helper_delete = tor_liste;
  while(helper_delete != nullptr)
  {  
    tore* to_delete = helper_delete;
    helper_delete = helper_delete ->next;
    delete to_delete;
  }

  return return_win;
}