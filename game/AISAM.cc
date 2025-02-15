#include "Player.hh"
#include <list>
#include <utility>


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME SAM


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */

  //PRE: entre p.i y p.j, como mucho hay uno que valga 0
  Dir pos_a_dir (Pos p) {
    if (p.i < 0) {
      if (p.j < 0) return TL; //EJ (-1, -1)
      else if (p.j > 0) return RT; //EJ (-1, 1)
      else return Top; //EJ (-1, 0)
    }

    else if (p.i > 0) {
      if (p.j < 0) return LB; //EJ (1, -1)
      else if (p.j > 0) return BR; //EJ (1, 1)
      else return Bottom; //EJ (1, 0)
    }

    else {
      if (p.j < 0) return Left; //EJ (0, -1)
      else return Right; //p.j < 0, por la pre, EJ (0, 1)

    } 
  }
  

 
  Dir BFS_enemigo(Pos mipos) { 
    // cerr << endl;
    // cerr << "SOY FURYANO BRRR" << endl << endl;

    vector<vector<bool>> visited (40, vector<bool> (80, false));
    vector<vector<int>> dist (40, vector<int> (80)); 
    //vector<vector<Dir>> mov (40, vector<Dir> (80));
    //mat dist (40, vector<int> (80, INF));
    //int dist_min = INF;
    // cerr << "ejecuto BFS_furyans" << endl;

    Dir direnemigo = None;
    Dir dirperraco = None;
    vector<pair<Pos,int>> perracos;
    queue <pair<Pos, Dir>> cola;
    cola.push({mipos, None});
    // cerr << mipos << endl;


    dist[mipos.i][mipos.j] = 0;
    int dist_act = 0;
    bool hay_perracos = false;


    visited[mipos.i][mipos.j] = true;
    bool found = false;
    bool first_it = true;
    while ((not cola.empty() and not found and not hay_perracos) or dist_act < 6) {
        int act_i = cola.front().first.i;
        int act_j = cola.front().first.j;
        Pos posact (cola.front().first.i, cola.front().first.j, 0);
        dist_act = dist[posact.i][posact.j];

        Dir dact = None;
        if (not first_it) { 
          dact = cola.front().second; 
          // cerr << "dact: "<< dact << endl; 
        }
        //int dist_act = dist[posact.i][posact.j];

        Cell cellposact = cell(posact);
        Unit unitposact;
        bool hay_alguien = false;
        // cerr << "en la pos " << posact << "esta la id " << cellposact.id << endl;
        if (cellposact.id != -1) {
          unitposact = unit(cell(posact).id);
          hay_alguien = true;
        }

        //verifico que no haya un Hellhound o un Furyan en mis cercanias
        if (hay_alguien) {
          // cerr << "hay alguien en " << posact << " y es un: ";
          //solo huyo de perros y de furyanos que me sacan vida y estan relativamente cerca
          if (unitposact.type == Hellhound or (unitposact.type == Furyan and unitposact.health > 25 + unit(cell(mipos).id).health and dist_act < 4)) { //si hay perracos huyo
            
            if (unitposact.type == Hellhound) perracos.push_back({posact,dist_act-2});
            else perracos.push_back({posact,dist_act});
            hay_perracos = true;
            // cerr << "yo estoy en " << mipos << endl;
            // cerr << "perraco en " << posact << endl;
          }
          else if (unitposact.player != me() and not found) { //si es un pionero o furyano ataco
            //enemigos.push_back({posact,dist_act});

            if(unitposact.type != Furyan or dist_act < 2) {
              found = true;
              direnemigo = dact;
            }
            else { //si furyano esta lejos tengo que tener ventaja para atacarle
              if (unit(cell(mipos).id).health > 25 + unitposact.health) {
                found = true;
                direnemigo = dact;
              }
            }
            // cerr << "yo estoy en " << mipos << endl;
            // cerr << "enemigo en " << posact << endl;
            //cerr << "furyano maloliente" << endl;
          }
        }






        // if (cellposact.id != -1 and unit(cellposact.id).health > 0 and unit(cellposact.id).player != me() and cellposact.type == Cave/*and dist_act < dist_min*/) {
        //   //dist_min = dist_act; //health == 0 para los perros!!!
        //   found = true;
        //   direnemigo = dact;
        //   // cerr << "encontrado:" << posact << cell(posact).type << ' ' << cell(posact).owner << endl;
        // }
        if (not found or dist_act < 6) {
          cola.pop();
          for (int i = 0; i<8; i++) { //probamos las 8 direcciones posibles
              Pos posnext = posact + Dir(i);
              // cerr << "posact: " << posact << endl;
              // cerr << "posnext: " << posnext << endl;
              Cell cellposnext = cell(posnext);
              if(pos_ok(posnext) and cellposnext.type == Cave and not visited[posnext.i][posnext.j]){
                  dist[posnext.i][posnext.j] = dist_act + 1;
                  if (first_it) {
                    // cerr << "primeras direcciones " << Dir(i) << endl;
                    // cerr << "i: " << i << "   first: " << first_it << endl;
                    cola.push({posnext,Dir(i)}); //nos queremos quedar solo con la primera dirección que toma el furyan
                    //en el camino para alcanzar al enemigo
                  }
                  else cola.push({posnext,dact});
                  visited[posnext.i][posnext.j] = true;
                  //mov[posnext.i][posnext.j] = Dir(i);
                  // cerr << posnext << cellposnext.type << ' ' << cellposnext.owner << Dir(i) << endl;
              }
              if (first_it and i == 7) first_it = false;
          }
        }

        if (dist_act == 6 and hay_perracos) found = true; //no hae falta que busque a quien atacar si me persiguen
    }

    // list <Dir> camino;
    // camino.push_front(mov[celdavacia.i][celdavacia.j]);
    
    // Pos posintermedia;
    // Pos posintermediaant = celdavacia;

    // Dir d;
    // if(mov[celdavacia.i][celdavacia.j] < 4) d = Dir(mov[celdavacia.i][celdavacia.j]+4);
    // else d = Dir(mov[celdavacia.i][celdavacia.j]-4);

    // posintermedia = celdavacia + d;
    // cerr << "posintermedia: " << posintermedia << endl;
    // cerr << "mov: " << mov[celdavacia.i][celdavacia.j] << endl;
    // cerr << "Dir: " << d << endl;

    // cerr << "hola2" << endl;

    // int i = 0; //para el cerr
    // while (posintermedia != mipos) {
    //     camino.push_front(mov[posintermedia.i][posintermedia.j]);
        // if (i < 10) cerr << posintermedia << endl;
        // posintermediaant = posintermedia;
        // if(mov[posintermedia.i][posintermedia.j] < 4) d = Dir(mov[posintermedia.i][posintermedia.j]+4);
        // else d = Dir(mov[posintermedia.i][posintermedia.j]-4);


        // ++i;
        // posintermedia = posintermedia + d; //al hacer el -4 invertimos la dirección
        //así que la linea es equivalente a posintermedia = posintermedia - mov[posintermedia.i][posintermedia.j];
        //lo hacemos porque en la API no hay función que nos permita restar direcciones
        //el -4 solo invierte direcciones bidimensionales!
    // }
    // cerr << "adios2" << endl;


    if(hay_perracos) {
      
      Pos res;
      int n_perracos = perracos.size();
      int dist_min = 10; //solo tenemos en cuenta a los enemigos mas cercanos
      for (int i = 0; i < n_perracos; ++i) {
        if (perracos[i].second < dist_min) dist_min = perracos[i].second;
      }
      //miramos el vector suma de las direcciones de donde nos vienen los enemigos, para luego desplazarnos al contrario
      for (int i = 0; i < n_perracos; ++i) {
        Pos mipos_neg(-mipos.i, -mipos.j, mipos.k);
        if (perracos[i].second == dist_min) res += perracos[i].first + mipos_neg; //quiero restar a enemigos.first mipos, asi que sumo mipos_neg
      }

      if (res.i == 0 and res.j == 0) { //hay dos enemigos en la misma recta o en las mismas rectas, huiremos a cualquier direccion que no esté en la recta
        
        if (n_perracos > 8) { //estas jodio
          dirperraco = direnemigo;
        }

        else { //hay salidas xD
          set<Dir> direcciones_enemigas;
          for (int i = 0; i < n_perracos; ++i) {
            direcciones_enemigas.insert(pos_a_dir(perracos[i].first));
          }

          bool hay_pos_vacia_huyendo = false; //mientras huimos intentamos invadir, tendre que hacer todas las iteraciones del bucle pero da igual
          for (int i = 0; i < 8 and not hay_pos_vacia_huyendo; ++i) {
            Cell c = cell(mipos + Dir(i));
            if (direcciones_enemigas.find(Dir(i)) != direcciones_enemigas.end()) ++i; //no tengo en cuenta las direcciones donde hay enemigos
            else if (c.owner != me() and c.id == -1 and c.type != Rock) {
              if(c.owner != me()) {
                dirperraco = Dir(i);
                hay_pos_vacia_huyendo = true;
              }
              else dirperraco = Dir(i);
            }
          }

          if (dirperraco == None) { //no puedo moverme a la direnemigo, ya que no he asignado una direnemigo en el bucle anteriori
            bool puedo_moverme = false;
            auto it = direcciones_enemigas.begin();
            while (not puedo_moverme and it == direcciones_enemigas.end()) {//pues tendre que moverme donde esta el enemigo
              Cell c = cell(mipos + *it);
              if (c.id == -1 and c.type != Rock) {
                dirperraco = *it;
                puedo_moverme = true;
              }
              ++it;
            }
          } //ya si no puedo hacer nada no me muevo xD
        }
      }
      else {

        Dir dir = pos_a_dir(res);
        if(dir < 4) {
          dirperraco = Dir(dir+4);
          Cell c = cell(mipos + dirperraco);
          if (c.type == Rock or c.id != -1) { //no me puedo mover en la dir contraria al enemigo
            found = false;

            for (int i = 0; i < 3 and not found; ++i) { //vamos probando en la direcciones contiguas a ver
              Dir d = Dir(dirperraco + Dir(i));
              if (d > 7) d = Dir(d-Dir(8));
              if (cell(mipos + d).type != Rock and cell(mipos + d).id == -1) {
                dirperraco = d;
                found = true;
              }
              else {
                d = Dir(dirperraco - Dir(i));
                if (d < 0) d = Dir(d+Dir(8));
                if (cell(mipos + d).type != Rock and cell(mipos + d).id == -1) {
                  dirperraco = d;
                  found = true;
                }
              }
            }

            if (not found) { //probamos la unica direccion que no consulta el bucle
              dirperraco = Dir(dirperraco + Dir(4));
              if (dirperraco == 8) dirperraco = Dir(0);
            }

          }

        }
        else {
          dirperraco = Dir(dir-4); //tenemos que ir a la direccion contraria a la del enemigo
          Cell c = cell(mipos + dirperraco);
          if (c.type == Rock or c.id != -1) { //no me puedo mover en la dir contraria al enemigo
            found = false;

            for (int i = 0; i < 3 and not found; ++i) { //vamos probando en la direcciones contiguas a ver
              Dir d = Dir(dirperraco + Dir(i));
              if (d > 7) d = Dir(d-Dir(8));
              if (cell(mipos + d).type != Rock and cell(mipos + d).id == -1) {
                dirperraco = d;
                found = true;
              }
              else {
                d = Dir(dirperraco - Dir(i));
                if (d < 0) d = Dir(d+Dir(8));
                if (cell(mipos + d).type != Rock and cell(mipos + d).id == -1) {
                  dirperraco = d;
                  found = true;
                }
              }
            }

            if (not found) { //probamos la unica direccion que no consulta el bucle
              dirperraco = Dir(dirperraco + Dir(4));
              if (dirperraco == 8) dirperraco = Dir(0);
            }

          }
        }

      // cerr << "MIPOS: " << mipos << "   dirperraco: " << dirperraco << endl;
      return dirperraco;
      }
    }

    //else 
    // cerr << "MIPOS: " << mipos << "   direnemigo: " << direnemigo << endl;
    return direnemigo;
    // cerr << endl << endl;

  }




  void move_furyans() {
    // cerr << "ejecuto move_furyans" << endl;
    // vector<int> perros = hellhounds();
    vector<int> myfuryans = furyans(me());
    int nfuryans = myfuryans.size();
    for (int i = 0; i < nfuryans; ++i) {
      Pos mi_pos = unit(myfuryans[i]).pos;

      // bool found = false;
      // for (int j = 0; j < 8 and not found; ++j) {
      //   //enum caveDir = mov_cueva();
      //   Pos next = mi_pos + Dir(j); //prueba a cave Dir
      //   Cell nextcell = cell(next);
      //   Unit nextunit;
      //   if (nextcell.id != -1) nextunit = unit(nextcell.id); //verifico que haya una unidad

      //   if (nextcell.type != Rock and nextcell.id != -1 and nextunit.player != me() and nextunit.player != -1) { //no hace verificar que sea hellhound o no porque ya estaria muerto si hubiese un perro
      //     command(myfuryans[i], Dir(j)); //prueba a cave Dir
          
      //     // cerr << endl;
      //     // cerr << "Pos act: " << mi_pos << "   " << "Pos next: " << next << endl;
      //     // cerr << "next owner: " << nextcell.owner << endl;
      //     found = true;
      //   }
      // }

      // cerr << "found: " << found << endl;

      // if (not found) {
        Dir denemigo = BFS_enemigo(mi_pos);
        // cerr << "Posicion " << mi_pos << endl;
        // cerr << "Movimiento " << denemigo << endl;
        command(myfuryans[i], denemigo);
      // }

    }

  }


 /* enum mov_cueva() {
    enum miDir {
      BR, RT, Right, TL, LB, Left, Bottom, Top,
      miDirSize
    }
  } */



  Dir BFS_posvacia(Pos mipos) { //hago bfs con mi pionero o un pionero enemigo??
    vector<vector<bool>> visited (40, vector<bool> (80, false));
    //vector<vector<Dir>> mov (40, vector<Dir> (80));
    vector<vector<int>> dist (40, vector<int> (80)); 
    //int dist_min = INF;

    Dir dirceldavacia = None;
    Dir direnemigo = None;
    vector<pair<Pos,int>> enemigos; //posicion y distancia de los enemigos, distancia hellhound sera 1 menor a la verdadera
    queue <pair<Pos, Dir>> cola;
    cola.push({mipos, None});
    // cerr << mipos << endl;

    dist[mipos.i][mipos.j] = 0;
    int dist_act = 0;
    bool hay_enemigos = false;

    visited[mipos.i][mipos.j] = true;
    bool found = false;
    bool first_it = true;
    // cerr << "te quedas en el while del bfs" << endl;
    while ((not cola.empty() and not found and not hay_enemigos) or dist_act < 6) {
        int act_i = cola.front().first.i;
        int act_j = cola.front().first.j;
        Pos posact (cola.front().first.i, cola.front().first.j, 0);
        dist_act = dist[posact.i][posact.j];
        // cerr << "distancia: " << dist_act << endl;
        // cerr << "distancia de la pos " << mipos << " a la pos " << posact << " es " << dist[posact.i][posact.j] << endl;

        Dir dact = None;
        if (not first_it) { 
          dact = cola.front().second; 
          // cerr << "dact: "<< dact << endl; 
        }

        Cell cellposact = cell(posact);
        Unit unitposact;
        bool hay_alguien = false;
        // cerr << "en la pos " << posact << "esta la id " << cellposact.id << endl;
        if (cellposact.id != -1) {
          unitposact = unit(cell(posact).id);
          hay_alguien = true;
        }

        //verifico que no haya un Hellhound o un Furyan en mis cercanias
        if (hay_alguien) {
          // cerr << "hay alguien en " << posact << " y es un: ";
          if (unitposact.type == Hellhound) {
            enemigos.push_back({posact,dist_act-2});
            hay_enemigos = true;
            // cerr << "perraco" << endl;
          }
          else if (unitposact.player != me() and unitposact.type == Furyan and dist_act < 4) {
            enemigos.push_back({posact,dist_act});
            hay_enemigos = true;
            // cerr << "furyano maloliente" << endl;
          }
        }

        // cerr << "aqui1?" << endl;

        if (cellposact.owner != me() and cellposact.type == Cave and not hay_alguien and not found) {
          //dist_min = dist_act;
          found = true;
          dirceldavacia = dact;
          // cerr << "celda vacia" << endl;
          // cerr << "encontrado:" << posact << cell(posact).type << ' ' << cell(posact).owner << endl;
        }

        if (not found or dist_act < 6) {
          cola.pop();
          for (int i = 0; i<8; i++) { //probamos las 8 direcciones posibles
              Pos posnext = posact + Dir(i);
              Cell cellposnext = cell(posnext);
              if(pos_ok(posnext) and cellposnext.type == Cave and not visited[posnext.i][posnext.j]){
                  dist[posnext.i][posnext.j] = dist_act + 1;
                  // cerr << "distancia en la pos next" << posnext << "  -> " << dist[posnext.i][posnext.j] << endl;
                  if (first_it) {
                    // cerr << "primeras direcciones " << Dir(i) << endl;
                    // cerr << "i: " << i << "   first: " << first_it << endl;
                    cola.push({posnext,Dir(i)}); //nos queremos quedar solo con la primera dirección que toma el furyan
                    //en el camino para alcanzar al enemigo
                  }
                  else cola.push({posnext,dact});

                  visited[posnext.i][posnext.j] = true;
                  //mov[posnext.i][posnext.j] = Dir(i);
                  // cerr << posnext << cellposnext.type << ' ' << cellposnext.owner << Dir(i) << endl;
              }
              if (first_it and i == 7) first_it = false;
          }
        }


        // cerr << "aqui3?" << endl;
        if (dist_act == 6 and hay_enemigos) found = true; //no hace falta que busque celda vacia, tengo que huir
    }

    // cerr <<"sales del while del BFS" << endl;
    // cerr << endl << endl;
    // cerr << "hay enemigos: " << hay_enemigos << endl;
    if(hay_enemigos) {
      Pos res;
      int n_enemigos = enemigos.size();
      int dist_min = 10; //solo tenemos en cuenta a los enemigos mas cercanos
      for (int i = 0; i < n_enemigos; ++i) {
        if (enemigos[i].second < dist_min) dist_min = enemigos[i].second;
      }
      //miramos el vector suma de las direcciones de donde nos vienen los enemigos, para luego desplazarnos al contrario
      for (int i = 0; i < n_enemigos; ++i) {
        Pos mipos_neg(-mipos.i, -mipos.j, mipos.k);
        if (enemigos[i].second == dist_min) res += enemigos[i].first + mipos_neg; //quiero restar a enemigos.first mipos, asi que sumo mipos_neg
      }

      if (res.i == 0 and res.j == 0) { //hay dos enemigos en la misma recta o en las mismas rectas, huiremos a cualquier direccion que no esté en la recta
        
        if (n_enemigos > 8) { //estas jodio
          direnemigo = dirceldavacia;
        }

        else { //hay salidas xD
          set<Dir> direcciones_enemigas;
          for (int i = 0; i < n_enemigos; ++i) {
            direcciones_enemigas.insert(pos_a_dir(enemigos[i].first));
          }

          bool hay_pos_vacia_huyendo = false; //mientras huimos intentamos invadir, tendre que hacer todas las iteraciones del bucle pero da igual
          for (int i = 0; i < 8 and not hay_pos_vacia_huyendo; ++i) {
            Cell c = cell(mipos + Dir(i));
            if (direcciones_enemigas.find(Dir(i)) != direcciones_enemigas.end()) ++i; //no tengo en cuenta las direcciones donde hay enemigos
            else if (c.owner != me() and c.id == -1 and c.type != Rock) {
              if(c.owner != me()) {
                direnemigo = Dir(i);
                hay_pos_vacia_huyendo = true;
              }
              else direnemigo = Dir(i);
            }
          }

          if (direnemigo == None) { //no puedo moverme a la direnemigo, ya que no he asignado una direnemigo en el bucle anteriori
            bool puedo_moverme = false;
            auto it = direcciones_enemigas.begin();
            while (not puedo_moverme and it == direcciones_enemigas.end()) {//pues tendre que moverme donde esta el enemigo
              Cell c = cell(mipos + *it);
              if (c.id == -1 and c.type != Rock) {
                direnemigo = *it;
                puedo_moverme = true;
              }
              ++it;
            }
          } //ya si no puedo hacer nada no me muevo xD
        }
      }
      else {

        Dir dir = pos_a_dir(res);
        if(dir < 4) {
          direnemigo = Dir(dir+4);
          Cell c = cell(mipos + direnemigo);
          if (c.type == Rock or c.id != -1) { //no me puedo mover en la dir contraria al enemigo
            found = false;

            for (int i = 1; i < 4 and not found; ++i) { //vamos probando en la direcciones contiguas a ver
              Dir d = Dir(direnemigo + Dir(i));
              if (d > 7) d = Dir(d-Dir(8));
              if (cell(mipos + d).type != Rock and cell(mipos + d).id == -1) {
                direnemigo = d;
                found = true;
              }
              else {
                d = Dir(direnemigo - Dir(i));
                if (d < 0) d = Dir(d+Dir(8));
                if (cell(mipos + d).type != Rock and cell(mipos + d).id == -1) {
                  direnemigo = d;
                  found = true;
                }
              }
            }

            if (not found) { //probamos la unica direccion que no consulta el bucle
              direnemigo = Dir(direnemigo + Dir(4));
              if (direnemigo == 8) direnemigo = Dir(0);
            }

          }

        }
        else {
          direnemigo = Dir(dir-4); //tenemos que ir a la direccion contraria a la del enemigo
          Cell c = cell(mipos + direnemigo);
          if (c.type == Rock or c.id != -1) { //no me puedo mover en la dir contraria al enemigo
            found = false;

            for (int i = 1; i < 4 and not found; ++i) { //vamos probando en la direcciones contiguas a ver
              Dir d = Dir(direnemigo + Dir(i));
              if (d > 7) d = Dir(d-Dir(8));
              if (cell(mipos + d).type != Rock and cell(mipos + d).id == -1) {
                direnemigo = d;
                found = true;
              }
              else {
                d = Dir(direnemigo - Dir(i));
                if (d < 0) d = Dir(d+Dir(8));
                if (cell(mipos + d).type != Rock and cell(mipos + d).id == -1) {
                  direnemigo = d;
                  found = true;
                }
              }
            }

            if (not found) { //probamos la unica direccion que no consulta el bucle
              direnemigo = Dir(direnemigo + Dir(4));
              if (direnemigo == 8) direnemigo = Dir(0);
            }

          }
        }

      // cerr << "MIPOS: " << mipos << "   direnemigo: " << direnemigo << endl;
      return direnemigo;
      }
    }

    //else 
    // cerr << "MIPOS: " << mipos << "   dirceldavacia: " << dirceldavacia << endl;
    return dirceldavacia;
    // cerr << endl << endl;


    // list <Dir> camino;
    // camino.push_front(mov[celdavacia.i][celdavacia.j]);
    
    // Pos posintermedia;
    // //Pos posintermediaant = celdavacia;

    // Dir d;
    // if(mov[celdavacia.i][celdavacia.j] < 4) d = Dir(mov[celdavacia.i][celdavacia.j]+4);
    // else d = Dir(mov[celdavacia.i][celdavacia.j]-4);

    // posintermedia = celdavacia + d;
    // cerr << "posintermedia: " << posintermedia << endl;
    // cerr << "mov: " << mov[celdavacia.i][celdavacia.j] << endl;
    // cerr << "Dir: " << d << endl;

    // cerr << "hola2" << endl;

    // int i = 0; //para el cerr
    // while (posintermedia != mipos) {
    //     camino.push_front(mov[posintermedia.i][posintermedia.j]);
        // if (i < 10) cerr << posintermedia << endl;
        // posintermediaant = posintermedia;
        // if(mov[posintermedia.i][posintermedia.j] < 4) d = Dir(mov[posintermedia.i][posintermedia.j]+4);
        // else d = Dir(mov[posintermedia.i][posintermedia.j]-4);


        // ++i;
        // posintermedia = posintermedia + d; //al hacer el -4 invertimos la dirección
        //así que la linea es equivalente a posintermedia = posintermedia - mov[posintermedia.i][posintermedia.j];
        //lo hacemos porque en la API no hay función que nos permita restar direcciones
        //el -4 solo invierte direcciones bidimensionales!
    //}
    // cerr << "adios2" << endl;
  }


  void move_pioneers() {
    //vector<int> perros = hellhounds();
    vector<int> mypioneers = pioneers(me());
    int npioneers = mypioneers.size();
    for (int i = 0; i < npioneers; ++i) {
      Pos mi_pos = unit(mypioneers[i]).pos;

      // bool found = false;
      // for (int j = 0; j < 8 and not found; ++j) {
      //   //enum caveDir = mov_cueva();
      //   Pos next = mi_pos + Dir(j); //prueba a cave Dir
      //   Cell nextcell = cell(next);
      //   if (nextcell.owner != me() and nextcell.type == Cave and nextcell.id == -1) {
      //     command(mypioneers[i], Dir(j)); //prueba a cave Dir
      //     found = true;
      //   }
      // }

      // if (not found) {
        Dir dceldavacia = BFS_posvacia(mi_pos);
        // cerr << "Posicion " << mi_pos << endl;
        // cerr << "Movimiento " << denemigo << endl;
        command(mypioneers[i], dceldavacia);
      // }

    }

  }



  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    move_pioneers();
    move_furyans();
    //cerr << "CPU consumida:" << status(me()) << endl;
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
