package trolle.af.wine.cellar.client

import io.ktor.client.HttpClient
import io.ktor.client.plugins.HttpTimeout
import io.ktor.client.plugins.contentnegotiation.ContentNegotiation
import io.ktor.serialization.kotlinx.json.json

actual fun getClient(): HttpClient =
    HttpClient {
        expectSuccess = true
        install(HttpTimeout) {
            socketTimeoutMillis = 500
            connectTimeoutMillis = 500
            requestTimeoutMillis = 500
        }
        install(ContentNegotiation) {
            json()
        }
    }