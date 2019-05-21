package com.example.myproject;

import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import org.opencv.android.Utils;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.Size;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.imgproc.Imgproc;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private Spinner spinner;
    private Button btnSubmit;
    private Mat image;
    private int num ;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        spinner =  findViewById(R.id.spinner1);

        btnSubmit =  findViewById(R.id.btnSubmit);

        final TextView sonuc = findViewById(R.id.resultTotalTime);

        final ImageView imageView = findViewById(R.id.imageView2);
        //System.out.println(Runtime.getRuntime().availableProcessors());


        btnSubmit.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Bitmap imageViewBitmap = ((BitmapDrawable)imageView.getDrawable()).getBitmap();
                image = bitmapToMat(imageViewBitmap);

                if (image != null) {
                    Mat m = image;

                    Imgproc.resize(m, m, new Size(m.rows()/2, m.rows()/2), 0, 0,
                            Imgproc.INTER_AREA);


                    double x = MaoJNI(m.getNativeObjAddr(),Integer.parseInt(spinner.getSelectedItem().toString()));
                    Bitmap bm1 = Bitmap.createBitmap(m.cols(), m.rows(),Bitmap.Config.ARGB_8888);
                    Utils.matToBitmap(m, bm1);
                    // ImageView iv1 = (ImageView) findViewById(R.id.iv1);
                    imageView.setImageBitmap(bm1);
                    String timing = Double.toString(x);

                    sonuc.setText(timing );
                }
                else {
                    Toast.makeText(MainActivity.this, "ChOOSE PHOTO FIRST!", Toast.LENGTH_SHORT).show();
                }





                  /*

                    Bitmap imageViewBitmap = ((BitmapDrawable)imageView.getDrawable()).getBitmap();
                    long a = bitmapToMat(imageViewBitmap).getNativeObjAddr();




                    final double resultTime = MaoJNI(a, 2);
                    sonuc.setText(resultTime +"");
                */
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
