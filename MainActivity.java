package com.example.pas;

import android.annotation.SuppressLint;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONException;
import org.json.JSONObject;

public class MainActivity extends AppCompatActivity {
    TextView textView1, textView2, textView3, textView4, textView5;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        textView1 = findViewById(R.id.textView1);
        textView2 = findViewById(R.id.textView2);
        textView3 = findViewById(R.id.textView3);
        textView4 = findViewById(R.id.textView4);
        textView5 = findViewById(R.id.textView5);

        final Handler handler = new Handler();
        Runnable updateRunnable = new Runnable() {
            @Override
            public void run() {

                fetchSectorState(1, textView1);
                fetchSectorState(2, textView2);
                fetchSectorState(3, textView3);
                fetchSectorState(4, textView4);
                fetchSectorState(5, textView5);

                handler.postDelayed(this, 2000);
            }
        };

        handler.post(updateRunnable);
    }

    // 각 섹터에 해당하는 인자
    private void fetchSectorState(final int sectorNumber, final TextView textView) {
        String url = "http://www.all-tafp.org/sector_" + sectorNumber + "_get.php";

        StringRequest stringRequest = new StringRequest(Request.Method.GET, url,
                new Response.Listener<String>() {
                    @SuppressLint("ResourceAsColor")
                    @Override
                    public void onResponse(String response) {
                        try {
                            JSONObject jsonResponse = new JSONObject(response);
                            boolean success = jsonResponse.getBoolean("success");
                            if (success) {
                                String state = jsonResponse.getString("state");

                                if (state.equals("에러")) {
                                    textView.setText(sectorNumber + "번 섹터 문제 발생!");
                                    textView.setTextColor(Color.RED);
                                } else if (state.equals("정상")) {
                                    textView.setText(sectorNumber + "번 섹터 정상...");
                                    textView.setTextColor(Color.YELLOW);
                                } else {
                                    textView.setText(sectorNumber + "번 섹터 상태 불명");
                                }
                            } else {
                                String message = jsonResponse.getString("message");
                                Toast.makeText(MainActivity.this, message, Toast.LENGTH_SHORT).show();
                            }
                        } catch (JSONException e) {
                            e.printStackTrace();
                            Toast.makeText(MainActivity.this, "JSON 파싱 오류", Toast.LENGTH_SHORT).show();
                        }
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        Toast.makeText(MainActivity.this, "서버 오류 발생", Toast.LENGTH_SHORT).show();
                        error.printStackTrace();
                    }
                });

        RequestQueue requestQueue = Volley.newRequestQueue(this);
        requestQueue.add(stringRequest);
    }
}
