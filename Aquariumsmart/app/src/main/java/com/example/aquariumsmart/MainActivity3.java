package com.example.aquariumsmart;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Patterns;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.example.aquariumsmart.ui.user;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.database.FirebaseDatabase;

public class MainActivity3 extends AppCompatActivity implements View.OnClickListener {

    private TextView banna, btn2_signup;
    private EditText username, Email, pass_word;
    private FirebaseAuth Auth;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main3);

        banna = (TextView) findViewById(R.id.banna);
        banna.setOnClickListener(this);
        btn2_signup = (Button) findViewById(R.id.sign);
        btn2_signup.setOnClickListener(this);
        Auth = FirebaseAuth.getInstance();
        username = findViewById(R.id.username);
        Email = findViewById(R.id.Email);
        pass_word = findViewById(R.id.password1);
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()){
            case R.id.banna:
                startActivity(new Intent(this,MainActivity.class));
                break;
            case R.id.sign:
                btn2_signup();
                break;
        }
    }

    private void btn2_signup() {
        String user_name = username.getText().toString().trim();
        String email = Email.getText().toString().trim();
        String password= pass_word.getText().toString().trim();
        if(user_name.isEmpty())
        {
            username.setError("username is empty");
            username.requestFocus();
            return;
        }

        if(email.isEmpty())
        {
            Email.setError("Email is empty");
            Email.requestFocus();
            return;
        }
        if(!Patterns.EMAIL_ADDRESS.matcher(email).matches())
        {
            Email.setError("Enter the valid email address");
            Email.requestFocus();
            return;
        }
        if(password.isEmpty())
        {
            pass_word.setError("Enter the password");
            pass_word.requestFocus();
            return;
        }
        if(password.length()<6)
        {
            pass_word.setError("Length of the password should be more than 6");
            pass_word.requestFocus();
            return;
        }
        btn2_signup.setVisibility(View.VISIBLE);
        Auth.createUserWithEmailAndPassword(email,password).addOnCompleteListener(new OnCompleteListener<AuthResult>()
        {
            @Override
            public void onComplete(@NonNull Task<AuthResult> task) {
                if(task.isSuccessful()) {
                    user user = new user(user_name, email,password);
                    FirebaseDatabase.getInstance().getReference("user")
                            .child(FirebaseAuth.getInstance().getCurrentUser().getUid())
                            .setValue(user).addOnCompleteListener(new OnCompleteListener<Void>() {
                        @Override
                        public void onComplete(@NonNull Task<Void> task) {
                            if (task.isSuccessful()) {
                                Toast.makeText(MainActivity3.this, "You are successfully Registered", Toast.LENGTH_SHORT).show();

                            } else {
                                Toast.makeText(MainActivity3.this, "You are not Registered! Try again", Toast.LENGTH_SHORT).show();

                            }
                        }
                    });
                }else{
                    Toast.makeText(MainActivity3.this, "You are not Registered Faild", Toast.LENGTH_SHORT).show();

                }


            }
        });

    }
}