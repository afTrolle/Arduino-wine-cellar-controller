package trolle.af.wine.cellar.client

internal fun Float.toTwoDecimalString(): String {
    val inital = this.toString()
    val index = (inital.indexOf(".").takeUnless { it == -1 }?.plus(2))?.coerceAtMost(inital.length)
        ?: inital.length
    return inital.take(index)
}