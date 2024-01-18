#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>

using namespace std;

/**
 * Score points by scanning valuable fish faster than your opponent.
 **/
struct creature
{
    int creature_id;
    int creature_x;
    int creature_y;
    int creature_vx;
    int creature_vy;
    bool scaned;
    int dest;
};
struct my_drone
{
    int drone_x;
    int drone_y;
    int emergency;
    int battery;
    vector<creature> creatures;
};
bool compar(creature it1,creature it2)
{
    return (it2.dest < it2.dest);
}
int main()
{
    int creature_count;
    cin >> creature_count; cin.ignore();
    for (int i = 0; i < creature_count; i++) {
        int creature_id;
        int color;
        int type;
        cin >> creature_id >> color >> type; cin.ignore();
    }

    // game loop
    while (1) {
        vector<int> scan_count;
        map<int, my_drone> my_drones;
        int my_score;
        cin >> my_score; cin.ignore();
        int foe_score;
        cin >> foe_score; cin.ignore();
        int my_scan_count;
        cin >> my_scan_count; cin.ignore();
        for (int i = 0; i < my_scan_count; i++) {
            int creature_id;
            cin >> creature_id; cin.ignore();
            scan_count.push_back(creature_id);
        }
        int foe_scan_count;
        cin >> foe_scan_count; cin.ignore();
        for (int i = 0; i < foe_scan_count; i++) {
            int creature_id;
            cin >> creature_id; cin.ignore();
        }
        int my_drone_count;
        cin >> my_drone_count; cin.ignore();
        for (int i = 0; i < my_drone_count; i++) {
            int drone_id;
            int drone_x;
            int drone_y;
            int emergency;
            int battery;
            cin >> drone_id >> drone_x >> drone_y >> emergency >> battery; cin.ignore();
            my_drone tmp_struct;
            tmp_struct.drone_x = drone_x;
            tmp_struct.drone_y = drone_y;
            tmp_struct.emergency = emergency;
            tmp_struct.battery = battery;
            my_drones[drone_id] = tmp_struct;
        }
        int foe_drone_count;
        cin >> foe_drone_count; cin.ignore();
        for (int i = 0; i < foe_drone_count; i++) {
            int drone_id;
            int drone_x;
            int drone_y;
            int emergency;
            int battery;
            cin >> drone_id >> drone_x >> drone_y >> emergency >> battery; cin.ignore();
        }
        int drone_scan_count;
        cin >> drone_scan_count; cin.ignore();
        for (int i = 0; i < drone_scan_count; i++) {
            int drone_id;
            int creature_id;
            cin >> drone_id >> creature_id; cin.ignore();
        }
        int visible_creature_count;
        cin >> visible_creature_count; cin.ignore();
        for (int i = 0; i < visible_creature_count; i++) {
            int creature_id;
            int creature_x;
            int creature_y;
            int creature_vx;
            int creature_vy;
            cin >> creature_id >> creature_x >> creature_y >> creature_vx >> creature_vy; cin.ignore();
            creature tmp_struct;
            tmp_struct.creature_vx = creature_vx;
            tmp_struct.creature_x = creature_x;
            tmp_struct.creature_y = creature_y;
            tmp_struct.creature_vy = creature_vy;
            tmp_struct.creature_id = creature_id;
            if(scan_count.empty() == false && find(scan_count.begin(), scan_count.end(), creature_id) != scan_count.end())
                tmp_struct.scaned = true;
            for(int j = 0; j < my_drones.size(); j++)
            {
                tmp_struct.dest = sqrt(pow((my_drones[j].drone_x - creature_x), 2) + pow((my_drones[j].drone_y - creature_y), 2));
                my_drones[j].creatures.push_back(tmp_struct);
            }
        }
        int radar_blip_count;
        cin >> radar_blip_count; cin.ignore();
        for (int i = 0; i < radar_blip_count; i++) {
            int drone_id;
            int creature_id;
            string radar;
            cin >> drone_id >> creature_id >> radar; cin.ignore();
        }
        for (int i = 0; i < my_drone_count; i++) {
            if(i == 0)
            {
                sort(my_drones[i].creatures.begin(), my_drones[i].creatures.end(), compar);
                vector<creature>::iterator it = my_drones[i].creatures.begin();
                // sort(it, my_drones[i].creatures.end());
                // my_drones[i].creatures.
                // sort(.begin(), my_drones[i].creatures.end(),compar);
                for(;it != my_drones[i].creatures.end();it++)
                {
                    cerr << it->dest << endl;
                    // if(find(scan_count.begin() , scan_count.end(), it->second.creature_id) == scan_count.end())
                    //     break;
                }
                cout << "WAIT 0" << endl;
            }

            // if(it != my_drones[0].dest.end())
            // {
            //     cout << "MOVE " << it->second.creature_x  << " " << it->second.creature_y  << " 1" << endl; // MOVE <x> <y> <light (1|0)> | WAIT <light (1|0)>
            // }
            // else
            // {
            // }
        }
    }
}