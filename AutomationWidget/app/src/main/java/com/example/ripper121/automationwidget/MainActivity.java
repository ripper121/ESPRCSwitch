package com.example.ripper121.automationwidget;

import android.content.SharedPreferences;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    Button saveButton;
    EditText urlText;
    TextView infoText;
    private String gatewayURL="";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        SharedPreferences settings = getSharedPreferences("PREFS_NAME", 0);
        gatewayURL = settings.getString("gatewayURL", "http://192.168.0.1");

        urlText=new EditText(this);
        urlText=(EditText)findViewById(R.id.editText);
        urlText.setText(gatewayURL);

        infoText=new TextView(this);
        infoText=(TextView)findViewById(R.id.textView);


        saveButton=(Button)findViewById(R.id.save);
        saveButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                SharedPreferences settings = getSharedPreferences("PREFS_NAME", 0);
                SharedPreferences.Editor editor = settings.edit();
                editor.putString("gatewayURL", urlText.getText().toString());
                editor.commit();
                infoText.setText("Settings Saved!");
            }
        });
    }
}
