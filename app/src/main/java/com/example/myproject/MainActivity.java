package com.example.myproject;

import android.annotation.SuppressLint;
import android.app.ProgressDialog;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import org.opencv.android.Utils;
import org.opencv.core.CvType;
import org.opencv.core.Mat;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private Spinner spinner;
    private Button btnSubmit;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final Spinner spinner = findViewById(R.id.spinner1);

        btnSubmit = (Button) findViewById(R.id.btnSubmit);

        final TextView sonuc = findViewById(R.id.resultTotalTime);

        final ImageView imageView = findViewById(R.id.imageView2);
        //System.out.println(Runtime.getRuntime().availableProcessors());





        btnSubmit.setOnClickListener(new View.OnClickListener() {
            @SuppressLint("StaticFieldLeak")
            @Override
            public void onClick(View view) {

                final ProgressDialog progressDialog = new ProgressDialog(MainActivity.this);
                progressDialog.setMessage("Devam eden işleminiz bulunmaktadır. Lütfen bekleyiniz..");
                progressDialog.show();

                new AsyncTask<Void, Void, Long>() {
                    @Override
                    protected Long doInBackground(Void... voids) {
                        Bitmap imageViewBitmap = ((BitmapDrawable)imageView.getDrawable()).getBitmap();
                         return bitmapToMat(imageViewBitmap).nativeObj;

                    }

                    @Override
                    protected void onPostExecute(Long aLong) {
                        final double resultTime = MaoJNI(aLong,Integer.parseInt(spinner.getSelectedItem().toString()));
                        progressDialog.dismiss();
                        sonuc.setText(resultTime +"");
                    }
                }.execute();


           }
        });

    }

    private Mat bitmapToMat(Bitmap bitmap){
        Mat mat = new Mat();
        Bitmap bCV_8U = bitmap.copy(Bitmap.Config.ARGB_8888, true);
        Utils.bitmapToMat(bCV_8U, mat);
        mat.convertTo(mat, CvType.CV_8U);
        return mat;
    }

    public native double MaoJNI(long a , int x);
}
