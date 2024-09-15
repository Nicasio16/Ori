package com.example.myapplication

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import co.tryterra.terraandroidrtlocal.GenerateAuthToken
import co.tryterra.terraandroidrtlocal.GenerateUserToken
import com.example.myapplication.ui.theme.MyApplicationTheme
import co.tryterra.terrartandroid.TerraRT
import co.tryterra.terrartandroid.enums.Connections
import co.tryterra.terrartandroid.enums.DataTypes

class MainActivity : ComponentActivity() {
    public lateinit var terraRT: TerraRT

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        this.terraRT = TerraRT(
            devId = "4actk-healthapp-testing-80xqarUDhD",
            referenceId = "testing",
            context = this,
        ) {suc ->
            GenerateAuthToken("4actk-healthapp-testing-80xqarUDhD", "rHjOsuy9Q0ZGfkfeNE05jMvDAx8lTvDK").getAuthToken { token ->
                this.terraRT.initConnection(token){succ ->
                    if (succ){
                        terraRT.startDeviceScan(Connections.BLE,  useCache = false){success ->
                            if (success){
                                GenerateUserToken("rHjOsuy9Q0ZGfkfeNE05jMvDAx8lTvDK", "4actk-healthapp-testing-80xqarUDhD", terraRT.getUserId()!!).getAuthToken{userToken ->
                                    terraRT.startRealtime(Connections.BLE, userToken!!, setOf(DataTypes.HEART_RATE))
                                }
                            }
                        }

                    }

                }
            }
        }
        enableEdgeToEdge()
        setContent {
            MyApplicationTheme {
                Scaffold(modifier = Modifier.fillMaxSize()) { innerPadding ->
                    Greeting(
                        name = "Android",
                        modifier = Modifier.padding(innerPadding)
                    )
                }
            }
        }
    }
}

@Composable
fun Greeting(name: String, modifier: Modifier = Modifier) {
    Text(
        text = "Hello $name!",
        modifier = modifier
    )
}

@Preview(showBackground = true)
@Composable
fun GreetingPreview() {
    MyApplicationTheme {
        Greeting("Android")
    }
}