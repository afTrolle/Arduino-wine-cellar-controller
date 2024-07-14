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

internal object CellarClient {

    private val client = HttpClient {
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

    internal suspend fun getWeather(url: String?) =
        url?.let {
            client.get {
                host = url
                url("/weather")
            }.body<Weather>()
        }

    internal suspend fun open(url: String?) =
        url?.let {
            client.post {
                host = url
                url("/open")
            }
        }

    internal suspend fun close(url: String?) =
        url?.let {
            client.post {
                host = url
                url("/close")
            }
        }

}

@Serializable
internal data class Weather(
    val temperature: Float? = null,
    val humidity: Float? = null,
)