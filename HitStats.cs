using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Firebase.Firestore;
using System.Threading.Tasks;
using UnityEngine.UI;
using System.Linq;
using System.Threading;
using Firebase.Auth;

public class HitStats : MonoBehaviour
{
    public int targetCollectedCount;
    public int hazardHitCount;
    public int wallHitCount;
    public bool collisionDetected;

    void OnTriggerEnter(Collider other)
    {
        if(other.gameObject.tag == "Target")
        {
            ++targetCollectedCount;
            collisionDetected = true;
            
        }
    }

    void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.tag == "Wall")
        {
            ++wallHitCount;
            collisionDetected = true;

        }
        else if(collision.gameObject.tag == "Hazard")
        {
            ++hazardHitCount;
            collisionDetected = true;
        }
        
    }
}
