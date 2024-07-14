package trolle.af.wine.cellar.client

import io.ktor.client.HttpClient
import io.ktor.client.call.body
import io.ktor.client.plugins.HttpTimeout
import io.ktor.client.plugins.contentnegotiation.ContentNegotiation
import io.ktor.client.request.get
import io.ktor.client.request.host
import io.ktor.client.request.post
import io.ktor.client.request.url
import io.ktor.serialization.kotlinx.json.json
import kotlinx.serialization.Serializable

object CellarClient {

    private val client = HttpClient {
        install(HttpTimeout) {
            socketTimeoutMillis = 500
            connectTimeoutMillis = 500
            requestTimeoutMillis = 500
        }
        install(ContentNegotiation) {
            json()
        }
    }

    suspend fun getWeather(url: String?) =
        url?.let {
            client.get {
                host = url
                url("/weather")
            }.body<Weather>()
        }

    suspend fun open(url: String?) =
        url?.let {
            client.post {
                host = url
                url("/open")
            }
        }

    suspend fun close(url: String?) =
        url?.let {
            client.post {
                host = url
                url("/close")
            }
        }
}

@Serializable
data class Weather(
    val temperature: Float? = null,
    val humidity: Float? = null,
)