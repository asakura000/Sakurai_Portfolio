using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;


public class CaneLengthSetting : MonoBehaviour
{

    public CaneProfileFromFB profile; // grabs variables from Firestore

    public float globalCaneLength;
    private float temp = 0;     //to store the length of previous length setting


    private void Start()  //map use only
    {
        //temp = transform.localScale.y * 2;
        // Invoke("ChangeCaneLength", 1f);

        Invoke("GetCaneLenFromFirestore", 0.7f);

        Invoke("ChangeCaneLength", 0.8f);
    }

    public void GetCaneLenFromFirestore()
    {
        globalCaneLength = profile.length;
        Debug.Log("Got cane length from FS");
    }

   
    public void ChangeCaneLength()
    {

        temp = transform.localScale.y * 2; //the length of prev. setting

        transform.localScale = new Vector3(transform.localScale.x, (float)(globalCaneLength / 2), transform.localScale.z);


        float yAxisChange = (globalCaneLength - temp) / 2;


        if (temp < 0)
        {
            yAxisChange *= (-1);        //if the new setting is less than the prev. one, the position has to move back
        }

        transform.Translate(new Vector3(0, yAxisChange, 0));   //translate the cane in terms of the position of arcane

        Debug.Log("Changed cane length");

    }
}

